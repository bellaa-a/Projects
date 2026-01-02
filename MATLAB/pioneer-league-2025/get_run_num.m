function run_num = get_run_num(suffix)
    foldername = 'PIONEER25_';
    if nargin < 1
        suffix = '';  % Default suffix is an empty string
    end
    foldername = strcat(foldername, suffix);
    folderlength = strlength(foldername);
    files = extractfield(dir('runs'), 'name');
    max_file_num = 0;
    for i = 1:length(files)
        filename = files{i};
        if (length(filename) >= folderlength) && strcmp(filename(1:folderlength), foldername)
            filenum = str2double(filename(folderlength+1:end));
            if (filenum > max_file_num)
                max_file_num = filenum;
            end
        end
    end
    run_num = max_file_num + 1;
end
