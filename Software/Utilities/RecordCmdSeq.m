function RecordCmdSeq( filename )
%RECORDCMDSEQ Summary of this function goes here
%   Detailed explanation goes here


    outfile = fopen(filename,'w');

    com_port = 'COM9';
    fprintf('Opening %s... \n',com_port)
    s = serial(com_port);
    set(s,'BaudRate',4800);
%     set(s,'Timeout',1);
    fopen(s);
    fprintf('Listening...\n');
    
    byte_buf = [];
    evalin('base','stop = false;')
    
    try
        while 1
            fprintf('Waiting for cmd...\n')
            if(s.BytesAvailable)
                out = fread(s,s.BytesAvailable);
                byte_buf = [byte_buf; out];
                if(~isempty(byte_buf))
                    fprintf('Received %d bytes: ',length(byte_buf))
                    for i = 1:length(out)
                        fprintf('%d ',byte_buf(i))
                    end
                    fprintf('\n');
                    pkt_len = byte_buf(6) + 7;
                    if(length(byte_buf) >= pkt_len)
                        fprintf('Found full pkt with len %d\n', pkt_len);
                        time_resp = input('Enter command execution time (in sec):');
                        timestamp = typecast(uint32(time_resp*1000),'uint8');
                        for i = length(timestamp):-1:1
                            fprintf(outfile,'%s',timestamp(i));
                            fprintf('%x ',timestamp(i));
                        end
                        for i = 1:length(byte_buf)
                            fprintf(outfile,'%s',byte_buf(i));
                            fprintf('%x ',byte_buf(i));
                        end
                        fprintf('\n ');
                        byte_buf = [];
                    end
                end
            end
            resp = input('End record [y]','s');
            if(strcmp(resp,'y'))
                break
            end
        end
    catch me
      disp( getReport( me, 'extended', 'hyperlinks', 'on' ) )
    end
        
    fclose(s);
    delete(s);
    clear s
    
    fclose(outfile);
end

