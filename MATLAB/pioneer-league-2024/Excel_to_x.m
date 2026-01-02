function Excel_to_x(Excel_path)
    team_names = {'OAK' 'CAL'	'BOI'	'IDF'	'OGD'	...
                  'GRR'	'MIS'	'GFV'	'BIL'	'RMV'};
    nteams = length(team_names);
    template= [% M  T   W   T   F   S   S   for 2024
            0	1	1	1	2	2	2	%5/20/24
            0	3	3	3	4	4	4	%5/27/24
            0	5	5	5	6	6	6	%6/3/24
            0	7	7	7	8	8	8	%6/10/24
            0	9	9	9	10	10	10	%6/17/24
            0	11	11	11	12	12	12	%6/24/24
            0	13	13	13	14	14	14	%7/1/24
            0	15	15	15	16	16	16	%7/8/24
            0	17	17	17	18	18	18	%7/15/24
            0	19	19	19	20	20	20	%7/22/24
            0	21	21	21	22	22	22	%7/29/24
            0	23	23	23	24	24	24	%8/5/24
            0	25	25	25	26	26	26	%8/12/24
            0	27	27	27	28	28	28	%8/19/24
            0	29	29	29	30	30	30	%8/26/24
            0	31	31	31	32	32	32	%9/2/24
			0	0	0	0	0	0	0	
    ];
    nslots=max(max(template));
    gamesperslot=zeros(1,nslots);
    for i=1:nslots
        gamesperslot(i)=sum(sum(template==i));
    end
    start_indices = cumsum(gamesperslot) - gamesperslot + 1;

    %% 
    Excel_cell = readcell(Excel_path, 'Range', 'B4:K114', 'Sheet', 'Schedule');
    Excel = zeros(size(Excel_cell));
    [num_rows, num_cols] = size(Excel_cell);
    for row = 1:num_rows
        for col = 1:num_cols
            if ischar(Excel_cell{row, col}) && Excel_cell{row, col}(1) == '@'
                team_name = Excel_cell{row, col}(2:end);
                team_index = find(strcmp(team_name, team_names));
                Excel(row, col) = team_index;
            elseif ischar(Excel_cell{row, col}) && strcmp(Excel_cell{row,col}, 'Off')
                Excel(row,col) = 0;
            else
                Excel(row, col) = col;
            end
        end
    end
    
    % Initialize a variable to store the filtered rows
    Excel = Excel(~all(Excel==0, 2),:);
    filteredForExcel = Excel(start_indices, :);
    
    %%
    % Initialize the solution vector x
    x = zeros(nslots * nteams * nteams, 1);
    
    % Iterate over each row in ForExcel
    for i = 1:size(filteredForExcel, 1)
        % Update the solution vector x based on the team assignments
        for j = 1:nteams
            stadium = filteredForExcel(i,j);    
            if stadium > 0
                x((i - 1) * nteams * nteams + (j - 1) * nteams + stadium) = 1;
            end
        end
    end

    writematrix(x, "runs/rafael_x.csv")
end