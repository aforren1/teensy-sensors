classdef PsychScreen
    properties
        black;
        white;
        gray;
        red;
        green;
        blue;
        reversed; % reversed == true == white background
        background_colour;
        text_colour;
        center_x;
        center_y;
        window; % The thing referred to by `Flip()` and co.
        dims; % dimensions of the window
        priority; % highest priority possible for screen
        ifi; % interflip interval
    end % end properties
    
    methods    
        function obj = PsychScreen(varargin)
        % Additional settings are `big_screen` and `skip_tests`
            AssertOpenGL;
            screens = Screen('Screens');
            scrn_num = max(screens);
            opts = struct('black', BlackIndex(scrn_num),...
                          'white', WhiteIndex(scrn_num),...
                          'gray', [190, 190, 190],...
                          'red', [255, 30, 63],...
                          'green', [97, 255, 77],...
                          'blue', [85, 98, 255],...
                          'reversed', false,...
                          'big_screen', false,...
                          'skip_tests', true);
            opts = CheckInputs(opts, varargin{:});
            
            obj.black = opts.black;
            obj.white = opts.white;
            obj.gray = opts.gray;
            obj.red = opts.red;
            obj.green = opts.green;
            obj.blue = opts.blue;
            obj.reversed = opts.reversed;
            
            if opts.skip_tests
                Screen('Preference', 'SkipSyncTests', 1);
                Screen('Preference', 'SuppressAllWarnings', 1);
            end
            
            if opts.reversed
                obj.background_colour = opts.white;
                obj.text_colour = opts.black;
            else
                obj.background_colour = opts.black;
                obj.text_colour = opts.white;
            end
            
            if opts.big_screen % big screen
                [obj.window, obj.dims] = Screen('OpenWindow', scrn_num,...
                                                 obj.background_colour);
            else
                [obj.window, obj.dims] = Screen('OpenWindow', scrn_num,...
                                                 obj.background_colour,...
                                                 [200 200 600 600]);
            end
            
            [obj.center_x, obj.center_y] = RectCenter(obj.dims);
            obj.dims = obj.dims(3:4);
            Screen('BlendFunction', obj.window, ...
                   'GL_SRC_ALPHA', 'GL_ONE_MINUS_SRC_ALPHA');
            obj.priority = MaxPriority(obj.window);
            obj.ifi = Screen('GetFlipInterval', obj.window);
            
        end % end PsychScreen
        
        function obj = CloseScreen(obj)
        % Closes the PsychScreen object
            sca;
            obj = [];
        end % end CloseScreen
        
        function WipeScreen(obj)
            Screen('FillRect', obj.window, obj.background_colour);
        end
        
        function FillScreen(obj, colour)
            Screen('FillRect', obj.window, obj.(colour));
        end
        
        function out_time = FlipScreen(screen, flip_time)
            if nargin < 2
                flip_time = 0;
            end
            out_time = Screen('Flip', screen.window, flip_time);
        end
  
    end % end methods
end % end classdef   