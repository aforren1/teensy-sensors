
% Timing example in Psychtoolbox (MATLAB, Octave)

%Screen('Preference', 'SkipSyncTests', 1);
Screen('Preference', 'SuppressAllWarnings', 1);

screens = Screen('Screens');

scrn.screen_num = max(screens);

scrn.white = WhiteIndex(scrn.screen_num);
scrn.black = BlackIndex(scrn.screen_num);
[scrn.window, scrn.size] = Screen('OpenWindow', scrn.screen_num,...
                                       scrn.white, [0 0 600 600]); %
HideCursor();
scrn.ifi = Screen('GetFlipInterval', scrn.window);

topPriorityLevel = MaxPriority(scrn.window);
num_secs = 5;
num_frames = round(num_secs / scrn.ifi);
waitframes = 1;
timeout = zeros(1, num_frames);
second_cnt = 1;
Priority(topPriorityLevel);
vbl = Screen('Flip', scrn.window);
for frame = 1:num_frames
    second_cnt = round((frame/num_frames)*num_secs);
    % if odd time (seconds)
    if rem(second_cnt, 2) == 1
        Screen('FillRect', scrn.window, scrn.black);
    else
        Screen('FillRect', scrn.window, scrn.white);
    end

    Screen('DrawingFinished', scrn.window);
    vbl = Screen('Flip', scrn.window, vbl + (waitframes - 0.5) * scrn.ifi);
    timeout(frame) = vbl;
end
Priority(0);
ShowCursor();
sca;
