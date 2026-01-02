function ProcForExcel = ProcForExcel(ForExcelPath)
team_names = {'OAK' 'CAL'	'BOI'	'IDF'	'OGD'	'GJJ'	...
    'GRR'	'MIS'	'GFV'	'BIL'	'NoCo'	'RMV'};
teams_to_remove = {'GJJ', 'NoCo'};

nteams = length(team_names);
cols_to_keep = setdiff(1:nteams, find(ismember(team_names, teams_to_remove)));

template= readmatrix("combined_template.csv");
nslots=max(max(template));
gamesperslot=zeros(1,nslots);
for i=1:nslots
    gamesperslot(i)=sum(sum(template==i));
end

ForExcel = readmatrix(ForExcelPath);
ProcForExcel = zeros(nslots,nteams);
row= 1;
slot = 1;
while slot <= nslots
    len = ForExcel(row, end);
    if len == 1
        row = row + 1;
        continue;
    elseif len ~= gamesperslot(slot)
        for t=1:nteams
            if ForExcel(row,t) == ForExcel(row+1,t) && ForExcel(row,t)==t
                ProcForExcel(slot,t) = t;
            end
        end
        row = row + 2;
    else
        for t=1:nteams
            if ForExcel(row,t)==t
                ProcForExcel(slot,t) = t;
            end
        end
        row = row + 1;
    end
    slot = slot + 1;
end

ProcForExcel = ProcForExcel(:,cols_to_keep);
for i=1:nslots
    for j=1:length(cols_to_keep)
        if ProcForExcel(i,j) > 0
            ProcForExcel(i,j) = 1;
        end
    end
end

