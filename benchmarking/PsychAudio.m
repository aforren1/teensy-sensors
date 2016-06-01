classdef PsychAudio

    properties
        pamaster;
        sound_handle;
        which_audio_fun;
    end
    
    methods
        function obj = PsychAudio(n_channels)
            try PsychPortAudio('Close');
            catch
                warning('No active audio device')
            end
            InitializePsychSound(1);
            
            % set sound reading function
            % octave classes and function handles are weird for now, so wait
            % until FillAudio to figure out the *actual* function
            if IsOctave || verLessThan('matlab', '8')
                obj.which_audio_fun = 'wavread';
            else
                obj.which_audio_fun = 'audioread';
            end
            
            % open in master mode with low latency (other audio devs may fail)
            obj.pamaster = PsychPortAudio('Open', [], 9, 2, 44100, 2);
            PsychPortAudio('Start', obj.pamaster, 0, 0, 1);
            obj.sound_handle = zeros(1, n_channels);
            for ii = 1:n_channels
                obj.sound_handle(ii) = PsychPortAudio('OpenSlave', obj.pamaster, 1);
            end
        end % end constructor
        
        function FillAudio(obj, file_or_matrix, aud_index)
        % obj is the object of PsychAudio class 
        % file_or_matrix is the complete path to the audio file (or an existing matrix)
        % aud_index is the index of the slave to fill
            if ischar(file_or_matrix)
                if strcmpi(obj.which_audio_fun, 'wavread')
                    audio_fun = @wavread;
                elseif strcmpi(obj.which_audio_fun,  'audioread')
                    audio_fun = @audioread;
                else
                    error('No good audio reading function (??)')
                end
                snd = audio_fun(file_or_matrix); % read sound w/ wavread or audioread
            elseif ismatrix(file_or_matrix)
                snd = file_or_matrix;
            else
                error('No recognized type for file_or_matrix')
            end
            % looking for a 2xn matrix, duplicate channel if mono          
            if size(snd, 1) > size(snd, 2)
                snd = snd';
            end
            if size(snd, 1) < 2
                snd = [snd; snd];
            end
            PsychPortAudio('CreateBuffer', aud_index, snd);
            PsychPortAudio('FillBuffer', aud_index, snd);
        end
        
        function out_time = PlayAudio(obj, aud_index, time)
            out_time = PsychPortAudio('Start', aud_index, 1, time, 1);
        end
        
        function StopAudio(obj, aud_index)
            PsychPortAudio('Stop', aud_index);
        end
        
        function CloseAudio(obj)
            PsychPortAudio('Close');
        end
    
    end

end