library(shiny) # web app framework (runs in browser)
library(shinythemes) # (optional)
library(serial)
library(tidyr) # reshaping data
library(dplyr) # filtering data
library(ggvis) # plotting

ser_conn <- serialConnection(name = "ardy", port = "ttyACM0",
                             mode = "9600,n,8,1") # make baud rate, etc. matches your arduino code
open(ser_conn) # returns success, regardless of actual outcome

# hacky way to try/catch; for whatever reason, I can't catch errors in external C code
if (nchar(geterrmessage() > 1)) {
  live <- 0
  message('No device, going into simulation...')
} else {
  live <- 1
}

if (live) {
  Sys.sleep(.1)
  d <-
    read.serialConnection(ser_conn) # exclude the header of the data -- difficult to plot text!
  Sys.sleep(.1)
  dat <-
    read.delim(textConnection(read.serialConnection(ser_conn)), header = FALSE)[1:3]
} else {
  set.seed(1)
  dat <- data.frame(
    V1 = 0,
    V2 = 0,
    V3 = 0,
    V4 = rnorm(1)
  )
  counter <- 0
}

ui <- shinyUI(fluidPage(
  theme = shinytheme("readable"),
  # theme is entirely optional
  titlePanel("Serial Port Mk 4"),
  
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
        max = 200,
        value = 80
      ),
      sliderInput("yaxis", "Y-axis limits:",-10, 10, c(-3, 3)),
      downloadButton('downloadData', 'Download')
    ),
    
    # show plot in the main panel
    mainPanel(uiOutput("ggvis_ui"),
              ggvisOutput("ggvis"))
  )
))

server <- shinyServer(function(input, output, session) {
  fetchData <- reactive({
    invalidateLater(1, NULL)
    if (live) {
      # getting data from the serial port
      dat <<- rbind(dat,
                    read.delim(textConnection(read.serialConnection(ser_conn)),
                               header = FALSE)[1:3])
    } else {
      # generating more fake data
      counter <<- counter + 1
      dat <<-
        rbind(dat,
              data.frame(
                V1 = 0.002 * counter,
                V2 = ifelse(counter %% 5 == 0, 2,-2),
                V3 = ifelse(counter %% 7 == 0,-1, 1),
                V4 = rnorm(1)
              ))
    }
    gather(dat, group, value,-V1) # from tidyr
  })
  
  fetchData %>%
    group_by(group) %>%
    arrange(V1) %>% # sort by time/sample number
    filter(row_number() >= n() - input$nsamples) %>% # display the `nsamples` most current samples
    filter(group %in% input$grps) %>%
    ggvis( ~ V1, ~ value, stroke = ~ group) %>%
    layer_lines() %>%
    scale_numeric(
      "y",
      domain = reactive(input$yaxis),
      nice = TRUE,
      clamp = TRUE
    ) %>%
    set_options(duration = 0.1) %>% # 0 makes the legend jump around, > 1 leads to weird transition effects
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

