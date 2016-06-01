
% Timing example in Psychtoolbox (MATLAB, Octave)
function [aud_out, vis_out] = ptb_benchmark
    try
        scrn = PsychScreen('big_screen', true);
        audio = PsychAudio(1);
        FillAudio(audio, 'beep.wav', 1);
        FillScreen(scrn, 'black');
        audio_times = GetSecs + (1:10);
        visual_times = audio_times - 0.5*scrn.ifi;
        aud_out = zeros(1,10);
        vis_out = zeros(1,10);
        FlipScreen(scrn);
        Priority(scrn.priority);
        for nn = 1:10
            if mod(nn, 2) == 1
                FillScreen(scrn, 'white');
            else
                FillScreen(scrn, 'black');
            end
            WaitSecs(0.6);
            StopAudio(audio, 1);
            aud_out(nn) = PlayAudio(audio, 1, audio_times(nn));
            vis_out(nn) = FlipScreen(scrn, visual_times(nn));
        end
        WaitSecs(0.6);
        Priority(0);
        CloseScreen(scrn);
        CloseAudio(audio);
    catch me
        CloseScreen(scrn);
        CloseAudio(audio);
        rethrow(me);
    end
end