function PIONEERreportgenerator_10(filename, problist, SchedInfo,template)
%% Setup
% Create file and open for writing
report = fopen(filename,'wt');

% find coinflip locations, set to series before for simplicity
[rcoinflips,ccoinflips] = find(template==-1);
coinflips = [rcoinflips ccoinflips];
for flip = 1:size(coinflips,1)
    template(coinflips(flip,1),coinflips(flip,2)) = ...
        template(coinflips(flip,1),coinflips(flip,2)+1) - 1;
end

% League Information
nslots = max(max(template));
OAK = 1; DAV = 2; BOI = 3; IDF = 4; OGD = 5; 
GJJ = 6; GRR = 7; MIS = 8; GFV = 9; BIL = 10;
CALI_TEAMS = [OAK DAV];
GROUP_1 = [DAV OAK];
GROUP_2 = [GJJ OGD];
GROUP_3 = [BOI IDF];
GROUP_4 = [GRR BIL MIS GFV];
WEST = [GROUP_1 GROUP_2];
EAST = [GROUP_3 GROUP_4];
nteams=max([WEST EAST]);
divisions = {WEST, EAST};
names = ["Oakland"
    "Yoho"
    "Boise"
    "Idaho Falls"
    "Ogden"
    "Grand Junction",
    "Glacier"
    "Missoula"
    "Great Falls"
    "Billings"];

% Convert series and off days to string dates
[series_dates, breaks_dates, breaksafter, dates] = get_dates(template);


% Matrix to convert Problist rows to text reports
% Order of errors below will be order of how they are reported
setup = {
%   name                    on/off	error#  slotcol	teamcol othercol
    'Home_Game_Total'       1       3       0       3       4
    'Home_Games_Per_Month'  1       51      0       0       0
    'Weekend_Game_Total'    1       4       2       3       0
    'Opening_Sequence'      0       15      2       3       0
    'July_3_4'              0       17      0       3       0
    'Team_Requests'         1       20      2       3       0
    'Team_Requests'         1       19      2       3       0 %
    'Off_Day_On_Road'       1       18      2       3       0
    'Closing_Sequence'      0       15      2       3       0
    'Long_Trips'            0       0       0       0       0
    'Unwanted_Trips'        1       6       2       3       4
    'Back_to_Back'          1       16      2       3       4 %
    'Three_Window'          1       10      2       3       0
    'Five_Window'           1       11      2       3       0
    'Seven_Window'          0       12      2       3       0
    'Three_Weekend'         1       13      2       3       0
    'All_Star_Before_After' 0       88      0       3       0
    'Opponent_Variety'      1       50      0       0       0 %
    'Original_Schedule'     0       21      0       0       0
    };

setup = cell2table(setup, 'VariableNames',...
    {'name' 'on' 'errornum' 'slotcol' 'teamcol' 'othercol'});
setup.name = string(setup.name);

%% Report Generation
% Print title
fprintf(report, "PIONEER League Schedule Analysis\n" ...
    + repmat('=', 1, 40) + "\n" + repmat('=', 1, 40) + "\n\n");

% Loop over all kinds of errors that were reported in Problist
for e = 1:size(setup,1)
    % Continue if report for constraint turned on
    if setup.on(e)
        % Grab column information from setup matrix
        name = setup.name(e);
        errornum = setup.errornum(e);
        slotcol = setup.slotcol(e);
        teamcol = setup.teamcol(e);
        othercol = setup.othercol(e);

        % Grab subset of Problist corresponding to the above contraint #
        errors = problist(problist(:,1)==errornum,:);

        % Call corresponding helper function to create text for
        % corresponding constraint
        switch name
            case 'Opening_Sequence'
                fprintf(report, Opening_Sequence(errors,slotcol,teamcol,othercol));
            case 'Home_Game_Total'
                fprintf(report, Home_Game_Total(errors,slotcol,teamcol,othercol));
            case 'Weekend_Game_Total'
                fprintf(report, Weekend_Game_Total(errors,slotcol,teamcol,othercol));
            case 'July_3_4'
                fprintf(report, July_3_4(errors,slotcol,teamcol,othercol));
            case 'Team_Requests'
                fprintf(report, Team_Requests(errors,slotcol,teamcol,othercol));
            case 'Off_Day_On_Road'
                fprintf(report, Off_Day_On_Road(errors,slotcol,teamcol,othercol));
            case 'Three_Window'
                fprintf(report, Three_Window(errors,slotcol,teamcol,othercol));
            case 'Five_Window'
                fprintf(report, Five_Window(errors,slotcol,teamcol,othercol));
            case 'Seven_Window'
                fprintf(report, Seven_Window(errors,slotcol,teamcol,othercol));
            case 'Three_Weekend'
                fprintf(report, Three_Weekend(errors,slotcol,teamcol,othercol));
            case 'Closing_Sequence'
                fprintf(report, Closing_Sequence(errors,slotcol,teamcol,othercol));
            case 'Long_Trips'
                fprintf(report, Long_Trips(errors,slotcol,teamcol,othercol));
            case 'All_Star_Before_After'
                fprintf(report, All_Star_Before_After(errors,slotcol,teamcol,othercol));
            case 'Unwanted_Trips'
                fprintf(report, Unwanted_Trips(errors,slotcol,teamcol,othercol));
            case 'Opponent_Variety'
                fprintf(report, Opponent_Variety(errors,slotcol,teamcol,othercol));
            case 'Back_to_Back'
                fprintf(report, Back_to_Back(errors,slotcol,teamcol,othercol));
            case 'Home_Games_Per_Month'
                fprintf(report, Home_Games_Per_Month(errors,slotcol,teamcol,othercol));
            case 'Original_Schedule'
                fprintf(report, Original_Schedule(errors,slotcol,teamcol,othercol));
            otherwise
                disp("Unknown error type listed in setup");
        end

        fprintf(report,'\n');
    end
end

% Close file for writing when done
fclose(report);

%% Nested Helper Functions
% Below are helper funcitons to print all errors for all constraints

    function [txt] = Home_Game_Total(errors,~,teamcol,othercol)
        txt = "Home Game Total Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                if errors(i,othercol)==1
                    txt = txt + get_team_name(errors(i,teamcol)) ...
                        + " plays too many home games.\n\n";
                end
            end
        end
    end

    function [txt] = Weekend_Game_Total(errors,slotcol,teamcol,~)
        txt = "Weekend Game Total Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                switch errors(i,slotcol)
                    case -1
                        txt = txt + get_team_name(errors(i,teamcol)) ...
                            + " has fewer than 9 weekend games at home.\n\n";
                    case 1
                        txt = txt + get_team_name(errors(i,teamcol)) ...
                            + " has more than 12 weekend games at home.\n\n";
                    case 3
                        txt = txt + get_team_name(errors(i,teamcol)) ...
                            + " has an invalid number of Thursday games at home.\n\n";
                    case 6
                        txt = txt + get_team_name(errors(i,teamcol)) ...
                            + " has fewer than 4 Saturday games at home.\n\n";
                    case 2
                        txt = txt + get_team_name(errors(i,teamcol)) ...
                            + " has too few weekend games in the first half.\n\n";
                    otherwise
                        continue
                end
            end
        end
    end

    function [txt] = Opening_Sequence(errors,slotcol,teamcol,~)
        txt = "Opening Sequence Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors) || sum(errors(:,slotcol)==1) == 0
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                if errors(i,slotcol) == 1
                    txt = txt + get_team_name(errors(i,teamcol)) ...
                        + " has an invalid opening sequence.\n\n";
                end
            end
        end
    end

    function [txt] = Closing_Sequence(errors,slotcol,teamcol,~)
        txt = "Closing Sequence Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors) || sum(errors(:,slotcol)==2) == 0
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                if errors(i,slotcol) == 2
                    txt = txt + get_team_name(errors(i,teamcol)) ...
                        + " has an invalid closing sequence.\n\n";
                end
            end
        end
    end

    function [txt] = July_3_4(errors,~,teamcol,~)
        txt = "July 3rd or 4th at Home Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                txt = txt + get_team_name(errors(i,teamcol)) ...
                    + " is not home for either July 3rd or July 4th.\n\n";
            end
        end
    end

    function [txt] = Long_Trips(errors,slotcol,teamcol,~)
        txt = "500 Mile Trips Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                txt = txt + get_team_name(team) ...
                    + " has an over-500 mile trip following Series " ...
                    + slot + " on " + series_dates(slot) + ".\n\n";
            end
        end
    end

    function [txt] = Unwanted_Trips(errors,slotcol,teamcol,othercol)
        txt = "Unwanted Trips Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            errors = sortrows(errors,2);
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                stad1 = SchedInfo(slot,team);
                stad2 = SchedInfo(slot+1,team);
                if stad1 < 0
                    stad1 = team;
                end
                if stad2 < 0
                    stad2 = team;
                end

                if errors(i,othercol) == 10
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot) + ...
                        ", an over 10-hour trip.\n\n";
                elseif errors(i,othercol) == 1
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot) + ...
                        ", a mid-week trip to/from California.\n\n";
                elseif errors(i,othercol) == 100
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot) + ...
                        ", an over 10-hour trip without off day.\n\n";
                elseif errors(i,othercol) == 101
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot) + ...
                        ", a mid-week trip to/from California by a non-airport city.\n\n";
                elseif errors(i,othercol) == 500
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot) + ...
                        ", an over 500-mile trip without an off-day.\n\n";
                else
                    txt = txt + get_team_name(team) ...
                        + " travels from " + get_team_name(stad1) + " to " ...
                        + get_team_name(stad2) + " following Series " + ...
                        + slot + " on " + series_dates(slot)+ ".\n\n";
                end
            end
            txt = txt + "Travel Violation Counts:\n";

            for team = unique(errors(:,teamcol))'
                txt = txt + "    " + get_team_name(team) + ': ' ...
                    + sum(errors(:,teamcol)==team) + '\n';
            end
            txt = txt + '\n';
        end

    end

    function [txt] = Team_Requests(errors,slotcol,teamcol,~)
        txt = "Team Requests Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                if SchedInfo(slot,team) < 0
                    txt = txt + get_team_name(team) ...
                        + " requested to be away during Series " + slot ...
                        + " which begins on " + series_dates(slot) ...
                        + ", but is home.\n\n";
                elseif SchedInfo(slot,team) > 0
                    txt = txt + get_team_name(team) ...
                        + " requested to be home during Series  " + slot ...
                        + " which begins on " + series_dates(slot) ...
                        + ", but is away.\n\n";
                end
            end
        end
    end

    function [txt] = Off_Day_On_Road(errors,slotcol,teamcol,~)
        txt = "Off Day on the Road Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                txt = txt + get_team_name(team) ...
                    + " is on the road on both sides of the off day on " ...
                    + breaks_dates(breaksafter==slot) + ".\n\n";
            end
        end
    end

    function [txt] = Three_Window(errors,slotcol,teamcol,~)
        txt = "3-Window Violation Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);

                if SchedInfo(slot,team) < 0
                    txt = txt + get_team_name(team) ...
                        + " has a 3-series homestand starting with Series " ...
                        + slot + " on " + series_dates(slot) + ".\n\n";
                elseif SchedInfo(slot,team) > 0
                    txt = txt + get_team_name(team) ...
                        + " has a 3-series roadtrip starting with Series " ...
                        + slot + " on " + series_dates(slot) + ".\n\n";
                end
            end
            txt = txt + "3-Window Violation Counts:\n";

            for team = unique(errors(:,teamcol))'
                txt = txt + "    " + get_team_name(team) + ': ' ...
                    + sum(errors(:,teamcol)==team) + '\n';
            end
            txt = txt + '\n';
        end
    end

    function [txt] = Five_Window(errors,slotcol,teamcol,~)
        txt = "5-Window Violation Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                numhome = sum(SchedInfo(slot:slot+4,team)<0);
                txt = txt + get_team_name(team) + " plays " ...
                    + numhome + " home series in a 5-series window" ...
                    + " starting with Series " + slot + " on " ...
                    + series_dates(slot) + ".\n\n";
            end
            txt = txt + "5-Window Violation Counts:\n";

            for team = unique(errors(:,teamcol))'
                txt = txt + "    " + get_team_name(team) + ': ' ...
                    + sum(errors(:,teamcol)==team) + '\n';
            end
            txt = txt + '\n';
        end
    end

    function [txt] = Seven_Window(errors,slotcol,teamcol,~)
        txt = "7-Window Violation Check \n"...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                numhome = sum(SchedInfo(slot:slot+6,team)<0);
                txt = txt + get_team_name(team) + " plays " ...
                    + numhome + " home series in a 7-series window" ...
                    + " starting with Series " + slot + " on " ...
                    + series_dates(slot) + ".\n\n";
            end
            txt = txt + "7-Window Violation Counts:\n";

            for team = unique(errors(:,teamcol))'
                txt = txt + "    " + get_team_name(team) + ': ' ...
                    + sum(errors(:,teamcol)==team) + '\n';
            end
            txt = txt + '\n';
        end
    end

    function [txt] = Three_Weekend(errors,slotcol,teamcol,~)
        txt = "3-Weekend Violation Check \n" ...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                %numhome = sum(SchedInfo(slot:slot+4,team)<0);
                txt = txt + get_team_name(team) + " plays " ...
                    + "an invalid number of home series in a 3-weekend window" ...
                    + " starting with Series " + slot + " on " ...
                    + series_dates(slot) + ".\n\n";
            end
        end
    end

    function [txt] = All_Star_Before_After(errors,~,teamcol,~)
        txt = "Home on Either Side of ASB Check \n" ...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                txt = txt + get_team_name(errors(i,teamcol)) ...
                    + " is away before and after the ASB.\n\n";
            end
        end
    end

    function [txt] = Back_to_Back(errors,slotcol,teamcol,othercol)
        txt = "Back to Back Series Check \n" ...
            + repmat('=', 1, 60) + "\n";
        if isempty(errors)
            txt = txt + "No violations to report.\n\n";
        else
            for i = 1:size(errors,1)
                slot = errors(i,slotcol);
                team = errors(i,teamcol);
                otherteam = errors(i,othercol);
                txt = txt + get_team_name(team) ...
                    + " plays " + get_team_name(otherteam) ...
                    + " in too many consecutive series starting with Series " ...
                    + slot + " on " + series_dates(slot) + ".\n\n";
            end
        end
    end

    function [txt] = Opponent_Variety(~,~,~,~)
        division_array = zeros(nteams, length(divisions));
        txt = "Variety of Opponents Check \n"...
            + repmat('=', 1, 60) + "\n";
        for team = 1:nteams
            otherteams = unique(abs(SchedInfo(:,team)));
            txt = txt + get_team_name(team) + ':' + " plays a total of " ...
                + length(otherteams) + " other teams.\n";
            if length(otherteams) == nteams -1
                txt = txt + '\n';
            else
                for notplay = setdiff(setdiff(1:nteams,team),otherteams)
                    txt = txt + "    **Team does not play " ...
                        + get_team_name(notplay) + '.**\n';
                end
            end

            for divIndex = 1:length(divisions)
                division = divisions{divIndex}; % Grab division array
                series_in_division = mean(ismember(abs(SchedInfo(:, team)), division));
                division_array(team, divIndex) = series_in_division;
            end
        end

        txt = txt + '\nPercent of Games Against Each Division\n' ...
            + repmat('=', 1, 60) + '\n' ...
            + formatTableToString(round(division_array,2), names, {'W', 'E'});
    end

    function [txt] = Home_Games_Per_Month(~,~,~,~)
        txt = "Home Games Per Month Check \n"...
            + repmat('=', 1, 60) + "\n";

        home_array = zeros(nteams,5);
        for week = 1:size(template(:,:,1),1)
            for day = 1:7
                if template(week,day) > 0
                    for t = 1:nteams
                        slot = template(week,day);
                        month = fix(dates(week,day)/100);

                        if SchedInfo(slot,t) < 0
                            home_array(t,month-4) = home_array(t,month-4) + 1;
                        end
                    end
                end
            end
        end

        months = {'May', 'Jun', 'Jul', 'Aug', 'Sept'};
        txt = txt + formatTableToString(home_array, names, months);
    end

    function [txt] = Original_Schedule(~,~,~,~)
        txt = "Comparison to Original Schedule \n"...
            + repmat('=', 1, 60) + "\n";

        real_sched = readmatrix('runs/PIONEER24_A12/PIONEER24_7_Sched.csv');
        new_sched = SchedInfo;
        base_template = zeros(size(template));
        for row = 1:size(template,1)-1
            base_template(row, :) = [0 repmat(2*row-1,1,3) repmat(2*row,1,3)];
        end
        [base_dates, ~, ~, ~] = get_dates(base_template);
        base_nslots = max(max(base_template));

        gamesperslot=zeros(1,base_nslots);
        for i=1:base_nslots
            gamesperslot(i)=sum(sum(template==i));
        end
        i = 1;
        row = 1;
        while i < base_nslots
            if gamesperslot(i) > 3
                new_sched(row + 1:end + 1, :) = new_sched(row:end, :);  % Shift rows down to make space
                new_sched(row, :) = new_sched(row + 1, :);  % Copy current row to the new row below
                row = row + 1;
            end
            i = i +1;
            row = row + 1;
        end
        assert (size(new_sched,1) == size(real_sched,1))
        real_sched = real_sched(:,[1:5 7:10 12]);


        txt = txt + "Changes from Home to Away:\n\n";
        for i = 1:size(new_sched,1)
            for j = 1:size(new_sched,2)
                if real_sched(i,j) < 0 && new_sched(i,j) > 0
                    txt = txt + get_team_name(j) ...
                        + " was originally home on " ...
                        + base_dates(i) ...
                        + ", but is away.\n";
                end
            end
        end
        txt = txt + "\nChanges from Away to Home:\n\n";
        for i = 1:size(new_sched,1)
            for j = 1:size(new_sched,2)
                if real_sched(i,j) > 0 && new_sched(i,j) < 0
                    txt = txt + get_team_name(j) ...
                        + " was originally away on " ...
                        + base_dates(i) ...
                        + ", but is home.\n";
                end
            end
        end

        home_kept = 1 - (sum(sum(real_sched < 0 & new_sched > 0)) / (base_nslots / 2 *10));
        txt = txt + "\nTotal Home Dates Met: " + sprintf('%s', num2str(home_kept * 100)) + '%%\n\n';
        txt = txt + "Home Dates Met Per Team (%%)\n";
        team_home_kept = 1 - (sum(real_sched < 0 & new_sched > 0, 1) / (base_nslots / 2));
        txt = txt + formatTableToString(team_home_kept' * 100, names, "");

    end

% Helper function to convert team number to city name
    function [teamname] = get_team_name(num)
        teamname = names(num) + " (Team " + num +")";
    end

    function formattedStr = formatTableToString(dataTable, rowNames, colNames)
        % Create column headers
        header = sprintf('%-20s', 'Team Name');  % Adjust spacing as needed
        for col = 1:numel(colNames)
            header = [header, sprintf('%-10s', colNames{col})];  % Adjust spacing as needed
        end
        header = [header, '\n'];

        % Convert dataTable to a formatted string
        formattedStr = header;
        for row = 1:size(dataTable, 1)
            rowStr = sprintf('%-20s', rowNames{row});  % Adjust spacing as needed
            for col = 1:size(dataTable, 2)
                value = dataTable(row, col);
                rowStr = [rowStr, sprintf('%-10s', num2str(value))]; % Adjust spacing as needed
            end
            formattedStr = [formattedStr, rowStr, '\n'];
        end
    end

%% Get Dates Function
% Helper function to get text dates for series and off days
    function [series_dates, breaks_dates, breaksafter, dates] = get_dates(template)
        monthz=5; dayz=20; % date of first day in template

        ndaysinmonthz=[
            31 % Jan
            28 % Feb
            31 % Mar
            30 % Apr
            31 % May
            30 % Jun
            31 % Jul
            31 % Aug
            30 % Sep
            31 % Oct
            30 % Nov
            31 % Dec
            ];
        nltemplate = size(template,1);
        dates = zeros(nltemplate,7);
        %This makes 3 digit code for dates
        for i=1:nltemplate
            for j=1:7
                dates(i,j)=100*monthz+dayz;
                if dayz==ndaysinmonthz(monthz)
                    monthz=monthz+1;
                    dayz=1;
                    if monthz==13
                        monthz=1;
                    end
                else
                    dayz=dayz+1;
                end
            end
        end

        breaksafter = getBreaksAfter(template);

        series_dates = strings(1,nslots);
        breaks_dates = strings(1,length(breaksafter));

        slot = 1;
        for i = 1:nltemplate
            for j = 1:7
                if template(i,j) == slot
                    firstrow = i;
                    firstcol = j;
                    datecode = dates(firstrow,firstcol);
                    month = fix(datecode/100);
                    day = rem(datecode,100);
                    series_dates(slot) = strcat(num2str(month),'/',num2str(day));

                    if sum(breaksafter== (slot -1)) > 0
                        breaknum = breaksafter == (slot -1);
                        if day == 1
                            month = month - 1;
                            day = ndaysinmonthz(month)+1;
                        end
                        breaks_dates(breaknum) = strcat(num2str(month),'/',num2str(day-1));
                    end

                    slot = slot +1;
                end
            end
        end
    end

    function [breaksafter] = getBreaksAfter(template)
        breaksafter = [];
        [rows,cols] = size(template);
        prev = 0;
        for r = 1:rows
            for c = 1:cols
                if template(r,c) == 0
                    if prev ~= 0
                        breaksafter = [breaksafter prev];
                        prev = template(r,c);
                    end
                    % if prev == 0, nothing to do
                else
                    prev = template(r,c);
                end
            end
        end
    end

end
