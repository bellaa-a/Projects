function run_num = get_run_num_all_teams()
    foldername = 'PIONEER24_All_Teams_';
    folderlength = length(foldername);
	files = extractfield(dir('runs'), 'name');
	max_file_num = 0;
	for i = 1:length(files)
		filename = files{i};
		if(length(filename) >= folderlength && strcmp(filename(1:folderlength),foldername))
			filenum = str2double(filename(folderlength+1:end));
			if (filenum > max_file_num)
				max_file_num = filenum;
			end
		end
	end
    run_num = max_file_num + 1;
end
			
