library(shiny) # web app framework (runs in browser)
#library(shinythemes) # (optional)
library(serial) # with my github version (https://github.com/aforren1/serial)
library(tidyr) # reshaping data
library(dplyr) # filtering data
library(ggvis) # plotting
library(tcltk)

ser_conn <- serialConnection(port = "ttyACM0",
                             mode = "9600,n,8,1") # make sure the baud rate, etc. matches arduino code
res <- try(open2(ser_conn), TRUE) 

if (class(res) == 'try-error') {
  live <- 0
  message('No device, going into simulation...')
} else {
  live <- 1
}

if (live) {
  Sys.sleep(.3)
  # exclude the header of the data -- difficult to plot text!
  invisible(read.serialConnection(ser_conn)) 
  Sys.sleep(.3)
  # read first bit as a starting point
  dat <- read.delim(textConnection(read.serialConnection(ser_conn)), header = FALSE)[1:3]
} else {
  set.seed(1)
  dat <- data.frame(
    V1 = 0,
    V2 = 0,
    V3 = 0,
    V4 = rnorm(1),
    V5 = runif(1, min = -5, max = 5)
  )
  counter <- 0
}

ui <- shinyUI(fluidPage(
  #theme = shinytheme("readable"),
  # theme is entirely optional
  titlePanel("Serial Port Mk 6"),
  
  # Sidebar with a slider input for number of bins
  sidebarLayout(
    sidebarPanel(
      checkboxGroupInput("grps", "Channels to use:",
                         names(dat)[-1], selected = names(dat[-1])),
      # all columns except first
      sliderInput(
        "nsamples",
        "Samples into the past:",
        min = 10,
        max = 500,
        value = 100
      ),
      sliderInput("yaxis", "Y-axis limits:",-10, 10, c(-5, 5)),
      downloadButton('downloadData', 'Download')
    ),
    
    # show plot in the main panel
    mainPanel(uiOutput("ggvis_ui"),
              ggvisOutput("ggvis"))
  )
))

server <- shinyServer(function(input, output, session) {
  fetchData <- reactive({
    invalidateLater(1, session)
    if (live) {
      # getting data from the serial port
      dat <<- rbind(dat,
                    read.delim(textConnection(read.serialConnection(ser_conn)),
                               header = FALSE)[1:3])
    } else {
      # generating more fake data
      counter <<- counter + 1
      V1temp <- 0.002 * (counter*20 + c(2:21))
      dat <<-
        rbind(dat,
              data.frame(
                V1 = V1temp,
                V2 = rep(ifelse(counter %% 23 == 0, 2,-2), 20),
                V3 = rep(ifelse(counter %% 7 == 0,-1, 1),20),
                V4 = rnorm(20), 
                V5 = rep(runif(1, min = -5, max = 5),20)
              ))
    }
    gather(dat, group, value,-V1) # from tidyr
  })
  
  fetchData %>%
    group_by(group) %>%
    arrange(V1) %>% # sort by time/sample number
    filter(row_number() >= n() - input$nsamples) %>% # `nsamples` = most current samples
    filter(group %in% input$grps) %>%
    ggvis( ~ V1, ~ value, stroke = ~ group) %>%
    layer_lines() %>%
    scale_numeric(
      "y",
      domain = reactive(input$yaxis),
      nice = TRUE,
      clamp = TRUE
    ) %>%
    set_options(duration = 0.001) %>% # 0 makes the legend jump around, > 1 leads to weird transition effects
    bind_shiny("ggvis", "ggvis_ui")
  
  output$downloadData <- downloadHandler(
    filename = function() {
      paste('data-', Sys.Date(), '.csv', sep = '')
    },
    content = function(file) {
      write.csv(dat, file)
    }
  )
  cancel.onSessionEnded <- session$onSessionEnded(function() {
    if (live) {
      close(ser_conn)
      message('Closed serial connection')
    }
    message('Session ended')
  })
})

# Run the application
shinyApp(ui = ui, server = server)

