function [ForExcel,SchedInfo,problist,nprobs]=PIONEERsched2025_all_teams(xstart)

addpath('/usr/local/gurobi/linux64/matlab')
if ischar(xstart) %make sure to put filename in single quotes
    xstart = csvread(xstart);
end

runnum=get_run_num("All_Teams_");

directory = ['runs/PIONEER25_All_Teams_' num2str(runnum)];

%% Define the constraints and choose Penalties
whichconstraints1=[
   %1  2   3   4    5      6     7      8     9   10   11   12   13   14  15   16      constraint
    1  1   1   1    1      1     0      0     1   1    0    0    1    0   0    1        %which
    0  0   0   0    1      1     1      1     1   1    1    1    1    1   1    1       %is slack
    0  0   1e5 1e5  1e4   1e2  7e-1    1e2  2e3  2e3  5e0  5e0  1e2  1e0 1e0  1e2     %penalty
    0  0   4   2    10      1     1      2     3  0    0    0    0    0   0    2  ];  %howmuch slack

whichconstraints2=[
  % 17  18  19  20  21  22  23
    0    0  1   1   1   1   1
    1    1  1   1   1   0   1
    1e4 1e3 1e1 1e5 1e3 0   1e4
    0    0  1   1   1   0   2];
whichconstraints = [whichconstraints1 whichconstraints2]

%constraints:
% C1: every team plays in every slot
% C2: every game is home vs visitor
% C3: every team has 48 home games
% C4: every team has home 8 weekends
% C5: distribution of games between teams
% C6: travel distance controls
% C7: every team home for one of first two slots, last two slots
% C8: Avoid short road trips for Ogden
% C9: If going to California, need to play at least two series then leave
% C10: no team is home three consecutive series or away; no three-window violations.
% C11: of every five consecutive series, we want 2 or 3 home
% C12: no pair of teams should play each other more than twice in 5 series
% C13: every three consecutive weekends, each team, number home weekends is one or two
% C14: of every five consecutive weekends, each team, number of weekends is two or three
% C15: of every seven consecutive weekends, each team, number of weekends is three or four
% C16: max 12 consecutive games between teams
% C17: no revisiting a stadium until w=2 intervening series pass
% C18: no traveling over a rest day
% C19: soft requests
% C20: hard requests
% C21: control 12-game homestands
% C22: max 27 games between any two teams
% C23: each team has to play everyone else in their group at least 18 times home or away

%% Define Schedule and Team requests

template= [% M  T   W   T   F   S   S   for 2025
             0	1	1	1	1   1   1	%5/19/25
             0	2   2   2   2   2   2	%5/26/25
             0	3   3   3   3   3   3	%6/2/25
             0	4   4   4	5   5   5	%6/9/25
             0	6   6   6   6   6   6	%6/16/25
             0	7   7   7   7   7   7	%6/23/25
             0	8   8   8   9   9   9	%6/30/25
             0	10  10  10  10  10  10	%7/7/25
             0	11  11  11  11  11  11	%7/14/25
             0	12  12  12  12  12  12	%7/21/25
             0	13  13  13  14  14  14	%7/28/25
             0	15  15  15  15  15  15	%8/4/25
             0	16  16  16  16  16  16	%8/11/25
             0	17  17  17  17  17  17	%8/18/25
             0	18  18  18	19  19  19	%8/25/25
             0	20  20  20  20  20  20	%9/1/25
 		  	 0	0	0	0	0	0	0	
     ];

% #days: 112; #week: 16
% #games: 96 -> 16 off days

[nltemplate,~]=size(template);
breaksafter=getBreaksAfter(template);

nslots=max(max(template));
gamesperslot=zeros(1,nslots);
for i=1:nslots
    gamesperslot(i)=sum(sum(template==i));
end
half=round(nslots / 2);

function series_set = get_series_set(start_slot, limit)
    series_set = start_slot;
    while sum(gamesperslot(series_set)) <= limit
        if max(series_set) == nslots
            break
        end
        series_set = [series_set max(series_set)+1];
    end
end

%% Team Setup
OAK = 1; MVL = 2; BOI = 3; IDF = 4; OGD = 5; GJJ = 6;
GRR = 7; MIS = 8; GFV = 9; BIL = 10; NCO = 11; RMV = 12;
CALI_TEAMS = [OAK MVL];
GROUP_1 = [RMV GJJ NCO];
GROUP_2 = [OGD BOI IDF];
GROUP_3 = [GRR MIS BIL GFV];
GROUP_4 = [MVL OAK];
WEST = [GROUP_1 GROUP_2];
EAST = [GROUP_3 GROUP_4];
nteams=max([WEST EAST]);
divisions = {WEST, EAST};
divn = [1 1 1 1 1 1 2 2 2 2 2 2];
Dist = [
 %  1    2    3    4    5    6    7    8    9   10    11   12
 %OAK  MVL  BOI  IDF  OGD  GJJ  GRR  MIS  GFV  BIL  NoCo  RMV
  0    117  633  824  764  962  1114 973  1184 1159 1202 1315 % OAK
  161  0    542  732  671  1062 1015 881  1089 1072 1107 1228 % MVL
  634  542  0    264  312  628  482  359  585  620  778  891  % BOI
  830  732  264  0    180  496  427  314  362  338  548  661  % IDF
  768  672  311  179  0    319  604  491  539  516  469  583  % OGD
  967  1067 627  495  319  0    920  807  855  663  298  311  % GJJ
  1119 1015 482  426  603  919  0    126  225  420  948  1061 % GRR
  1070 880  359  313  490  806  125  0    169  345  852  965  % MIS
  1189 1089 584  362  539  855  225  169  0    218  726  839  % GFV
  1164 1067 599  340  515  662  420  345  218  0    508  621  % BIL
  1201 1106 775  546  467  298  947  851  725  507  0    126  % NoCo
  1315 1224 889  659  580  310  1061 965  838  621  125  0    % RMV
    ];
TravelTimes = [
%  1     2     3     4     5     6     7     8     9    10     11   12     
%OAK   MVL   BOI   IDF   OGD   GJJ   GRR   MIS   GFV   BIL   NoCo  RMV
0.00  2.22  9.92  12.25 11.25 14.75 17.67 16.67 17.50 17.67 17.50 19.33 % OAK
2.60  0.00  8.58  11.07 9.73  15.50 16.77 15.52 16.23 16.68 16.40 18.47 % MVL
9.92  8.68  0.00  4.25  4.67  9.42  9.50  7.17  9.17  9.58  11.67 13.42 % BOI
12.50 11.03 4.33  0.00  2.67  7.50  6.75  4.75  5.75  5.67  8.83  10.58 % IDF
11.25 9.87  4.75  2.67  0.00  5.00  9.42  7.33  8.17  8.25  7.33  9.25  % OGD
15.25 16.00 9.42  7.42  5.00  0.00  14.08 11.75 12.67 10.17 5.00  5.17  % GJJ
17.92 16.87 9.50  6.75  9.17  14.00 0.00  2.33  3.75  7.00  14.25 16.17 % GRR
16.75 15.48 7.33  4.83  7.25  12.08 2.33  0.00  2.83  5.33  12.58 14.33 % MIS
17.75 16.83 9.08  5.58  8.00  12.83 3.75  2.83  0.00  3.67  11.00 12.75 % GFV
18.00 16.52 9.75  5.67  8.08  10.50 7.17  5.42  3.75  0.00  7.75  9.67  % BIL
17.83 16.53 11.58 8.83  7.25  5.08  14.83 12.58 11.17 7.67  0.00  2.08  % NoCo
19.33 18.20 13.25 10.58 8.83  5.25  16.33 14.08 12.67 9.25  2.08  0.00  % RMV
    ];

Airports = [BOI OGD NCO RMV GRR];

%% Conversion to Dates
% date of first day in template, top left entry whether a game day or not
monthz=5; dayz=19;

ndaysinmonthz=[
    31 % Jan
    28 % Feb
    31 % Mar
    30 % Apr
    31 % May
    30 % Jun
    31 % Julf
    31 % Aug
    30 % Sep
    31 % Oct
    30 % Nov
    31 % Dec
    ];

dates=zeros(nltemplate,7);
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

% Find series number for important dates
[i,j]=find(dates==704);
july4=template(i,j); %july4 is the series number

%% Restricted Dates
% team, homeaway, month, day;
% -1 means home, 1 means away, 2 means away from home but out of division
DATEShardrestricted=[
   IDF  1  5  20
   IDF  1  5  21
   IDF  1  5  22
   IDF  1  5  23
   IDF  1  5  24
   GRR  1  5  29
   GRR  1  5  30
   GRR  1  5  31
   GRR  1  6  1
   GRR  1  8  11
   GRR  1  8  12
   GRR  1  8  13
   GRR  1  8  14
   GRR  1  8  15
   GRR  1  8  16
   GRR  1  8  17
   GJJ  1  5  20
   GJJ  1  5  21
   GJJ  1  5  22
   GJJ  1  5  23
   GJJ  1  5  24
   GJJ  1  5  25
   GJJ  1  5  26
   GJJ  1  5  27
   GJJ  1  5  28
   GJJ  1  5  29
   GJJ  1  5  30
   GJJ  1  5  31
   GJJ  1  6  1
   GJJ  1  6  2
   GJJ  1  8  28
   GJJ  1  8  29
   GJJ  1  8  30
   GJJ  1  9  4
   GJJ  1  9  5
   GJJ  1  9  6
   BOI  1  6  21
   BOI  1  7  11
   BOI  1  7  12
   BOI  1  8  14
   BOI  1  8  15
   BOI  1  8  16
   BOI  1  8  17
   BOI  1  8  18
   BOI  1  8  19
   BOI  1  8  20
   BOI  1  8  21
   BOI  1  8  22
   BOI  1  8  23
   MIS  1  5  22
   MIS  1  5  23
   MIS  1  5  24
   MIS  1  8  30
   MIS  1  8  31
   OGD  1  5  21
   OGD  1  5  22
   OGD  1  5  23
   OGD  1  5  24
   OGD  1  6  6
   OGD  1  6  7
   OGD  1  6  19
   OGD  1  7  3
   OGD  1  7  4
   OGD  1  7  5
   OGD  1  7  14
   OGD  1  7  15
   OGD  1  7  16
   OGD  1  7  17
   OGD  1  7  18
   OGD  1  7  31
   OGD  1  8  1
   OGD  1  8  2
   OGD  1  8  11
   OGD  1  8  12
   OGD  1  8  13
   OGD  1  8  14
   OGD  1  8  15
   OGD  1  8  29
   OGD  1  8  30
   OGD  1  8  31
    ];

DATESsoftrestricted=[    
   IDF  -1 7  4
   GRR  1  5  23
   GRR  1  5  24
   GRR  1  5  25
   GRR  -1 6  15
   GRR  -1 7  4
   GRR  1  8  29
   GRR  1  8  30
   GRR  1  8  31
   GRR  1  9  1
   BOI  1  8  24
   BOI  -1 6  10
   BOI  -1 6  11
   BOI  -1 6  12
   BOI  -1 6  13
   BOI  -1 6  14
   BOI  -1 6  15
   BOI  -1 7  1
   BOI  -1 7  2
   BOI  -1 7  3
   BOI  -1 7  4
   BOI  -1 7  5
   BOI  -1 7  6
   MIS  -1 6  19
   %THIS
   MIS  -1 6  27
   MIS  -1 6  28
   %OR
   %MIS  -1 7  3
   %MIS  -1 7  4
    ];

[nHR,~]=size(DATEShardrestricted);
HARDWIRE=zeros(nslots,nteams);

[nSO,~]=size(DATESsoftrestricted);
SOFTWIRE=zeros(nslots,nteams);

% Convert hardrestricted dates to series
for k=1:nHR
    [i,j]=find(dates== 100* DATEShardrestricted(k,3)+ DATEShardrestricted(k,4) );
    slott=template(i,j);
    if slott>0
        if DATEShardrestricted(k,2)==1
            HARDWIRE(slott,DATEShardrestricted(k,1))=-100; %away
        elseif DATEShardrestricted(k,2)==2
            HARDWIRE(slott,DATEShardrestricted(k,1))=-101; %away out of  div
        else
            HARDWIRE(slott,DATEShardrestricted(k,1))=DATEShardrestricted(k,1);
        end
    end
end
for k=1:nSO
    [i,j]=find(dates== 100* DATESsoftrestricted(k,3)+ DATESsoftrestricted(k,4) );
    slott=template(i,j);
    if slott>0
        if DATESsoftrestricted(k,2)==1
            SOFTWIRE(slott,DATESsoftrestricted(k,1))=-100; %away
        elseif DATESsoftrestricted(k,2)==2
            SOFTWIRE(slott,DATESsoftrestricted(k,1))=-101; %away out of  div
        else
            SOFTWIRE(slott,DATESsoftrestricted(k,1))=DATESsoftrestricted(k,1);
        end
    end
end

%% Matrix Setup
numconstrainttypes=length(whichconstraints);
howmanyeq=zeros(1,numconstrainttypes);
howmanyineq=zeros(1,numconstrainttypes);
howmanycolumns=zeros(1,numconstrainttypes);

cushionrows=50000;
cushioncolumns=50000;

Ain=spalloc(cushionrows,nslots*nteams*nteams+cushioncolumns,cushioncolumns*8);
Aeq=spalloc(cushionrows,nslots*nteams*nteams+cushioncolumns,cushioncolumns*2);
bin=zeros(cushionrows,1);
beq=zeros(cushionrows,1);
f=zeros(nslots*nteams*nteams+cushioncolumns,1);

TRin=zeros(cushionrows,4);
TReq=zeros(cushionrows,4);

rin=0;                                   % index of row for Ain
req=0;                                   % index of row for Aeq
cols=nslots*nteams*nteams;               % index of columns for Ain/Aeq

%% Begin Constraints

%% C1; every team plays in every slot
if whichconstraints(1,1)==1 % if constraint 1 is active
    for i=1:nslots % in slot i
        for j=1:nteams % for team i
            req=req+1;
            for k=1:nteams % sum of elements at position i,j,k:1~n is 1
                Aeq(req,(i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)=1;
            end
            beq(req,1)=1;
        end
    end
end
completed=0;
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C2; every game is home vs visitor
if whichconstraints(1,2)==1
    for i=1:nslots % in series i
        for k=1:nteams
            req=req+1;
            for j=1:nteams
                if j==k % if team k holds game at home
                    Aeq(req,(i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)=1; % the sum over elements at positions i,j,k:1~n is 1
                else % if team k doesn't hold game at home
                    Aeq(req,(i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)=-1; % no game is holding at statium k
                end
            end
            beq(req,1)=0;
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C3; every team has 48 home games, at least 18 per half
if whichconstraints(1,3)==1
    for j=1:nteams % for team j
        rin=rin+1;
        for i=1:nslots
            Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(i); % sum over # games when j is at home
        end
        bin(rin,1)=-48; % the sum of #games at home in total >= 48
        TRin(rin,:)=[3 inf j inf];
        if whichconstraints(2,3)==1 
            for qq=1:whichconstraints(4,3)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,3); 
            end
        end
        rin=rin+1;
        for i=1:half % in the first half of series
            Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(i); % sum over # games when j is at home
        end
        bin(rin,1)=-18; % the sum of #games at home in first half >= 18
        TRin(rin,:)=[3 inf j 1];
        if whichconstraints(2,3)==1
            for qq=1:whichconstraints(4,3) 
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,3); 
            end
        end

        rin=rin+1; 
        for i=(half+1):nslots % in the second half of series
            Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(i); % the sum of #games at home in total is 24
        end
        bin(rin,1)=-18; % the sum of #games at home in second half >= 18
        TRin(rin,:)=[3 inf j 2];
        if whichconstraints(2,3)==1 
            for qq=1:whichconstraints(4,3) 
                cols=cols+1; 
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,3); 
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C4; every team has home 8 weekends
% Note - simplified due to no split weekends
if whichconstraints(1,4)==1
    for j=1:nteams
        rin=rin+1;
        for z=1:(nltemplate-1)
            i=template(z,5); % series# for Friday
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        end
        bin(rin,1)=-8;
        TRin(rin,:)=[4 inf j inf];
        if whichconstraints(2,4)==1 % compute penalty*slack
            for qq=1:whichconstraints(4,4)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,4);
                
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C5; distribution of games between teams
if whichconstraints(1,5)==1
    % every team plays every other team at least 3 games
    for j=[MIS]
        for jj=setdiff(1:nteams, j)
            rin = rin + 1;
            for i=1:nslots
                Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-gamesperslot(i);
                Ain(rin, (i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-gamesperslot(i);
            end
            bin(rin,1)=-3;
            TRin(rin,:)=[5 inf j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5) % compute slack*penalty
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                end
            end
        end
    end

    % Inland Teams can only visit each Cali Team for 6 games
    for j=setdiff(1:nteams, CALI_TEAMS)
        for jj = CALI_TEAMS
        	rin = rin + 1;
            for i = 1:nslots
                Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=gamesperslot(i);
            end
            bin(rin,1)=6;
            TRin(rin,:)=[5 6 j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5) % compute slack*penalty
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                end
            end
        end
    end

    % % Teams must visit all teams within 500 miles at least once
    % for j = 1:nteams
    %     for jj = find(Dist(j,:) <= 500)
    %         rin = rin + 1;
    %         for i=1:nslots
    %             Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
    %         end
    %         bin(rin,1)=-1;
    %         TRin(rin,:)=[5 500 j jj];
    %         if whichconstraints(2,5)==1
    %             for qq=1:whichconstraints(4,5) % compute slack*penalty
    %                 cols=cols+1;
    %                 Ain(rin,cols)=-qq;
    %                 f(cols,1)=qq*whichconstraints(3,5);
    %             end
    %         end
    %     end
    % end

    % Can't visit another team more than 18 times
    for j = 1:nteams
        for jj = setdiff(1:nteams,j)
            rin = rin + 1;
            for i=1:nslots
                Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=gamesperslot(i);
            end
            bin(rin,1)=18;
            TRin(rin,:)=[5 18 j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5) % compute slack*penalty
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                end
            end
        end
    end
end


completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C6 Travel Distance Controls - California Trips, 10 Hour and 500 Mile Rule
if whichconstraints(1,6)==1
    % Iterate through all teams
    for k=1:nteams
        for kk=1:nteams
            % If traveling to Cali teams, no penalty for flying
            if (ismember(k, Airports) && ismember(kk, CALI_TEAMS)) || (ismember(kk, Airports) && ismember(k, CALI_TEAMS))
                continue
            % If over 10 hour trip, higher penalty
            elseif TravelTimes(k,kk) > 10
                for i=1:(nslots-1)
                    for j=1:nteams
                        rin=rin+1;
                        Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(k -1)+1)=1;
                        Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(kk-1)+1)=1;
                        bin(rin,1)=1;
                        TRin(rin,:)=[6 i j 10];
                        if whichconstraints(2,6)==1
                            cols=cols+1;
                            Ain(rin,cols)=-1;
                            if ~ismember(i, breaksafter) % Very long trip without off day
                                f(cols,1)=100 * whichconstraints(3,6);
                                TRin(rin,:)=[6 i j 100];
                            elseif ismember(k, CALI_TEAMS) || ismember(kk, CALI_TEAMS)
                                % Bus to airport to get to California
                                f(cols,1)=10 * whichconstraints(3,6);
                                TRin(rin,:)=[6 i j 101];
                            else
                                f(cols,1)=whichconstraints(3,6);
                            end
                        end
                    end
                end
            % Distance greater than 500 miles without off day; apply regular penalty
            elseif Dist(k,kk)>500
                for i=1:(nslots-1)
                    if ~ismember(i, breaksafter)
                        for j=1:nteams
                            rin=rin+1;
                            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(k -1)+1)=1;
                            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(kk-1)+1)=1;
                            bin(rin,1)=1;
                            TRin(rin,:)=[6 i j 500];
                            if whichconstraints(2,6)==1
                                cols=cols+1;
                                Ain(rin,cols)=-1;
                                f(cols,1)=whichconstraints(3,6);
                            end
                        end
                    end
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C7
%     Every team is home first or second slot
%     Every team is home last or secondlast slot
if whichconstraints(1,7)==1
    for j=1:nteams % for every team
        req=req+1; % first and second series at home
        Aeq(req,(1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
        Aeq(req,(2-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
        beq(req,1)=1; %for >= 1 time
        req=req+1;
        Aeq(req,(nslots-1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1; % last and second to last series at home
        Aeq(req,(nslots  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
        beq(req,1)=1; %for >= 1 time
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C8: Avoid 1-series road trips for Ogden
if whichconstraints(1,8) == 1
    for j=OGD
        for i=1:nslots-2
            % Single Series Roadtrip
            rin=rin+1;
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            Ain(rin,(i+2-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
            bin(rin,1)=1;
            TRin(rin,:)=[8 i j 1];
            if whichconstraints(2,8)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,8);
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C9; if going to Cali, play at least two series then do not come back
if whichconstraints(1,9)==1
    % If going to Cali, play at least 9 games
    for i=1:nslots
        series_set = get_series_set(i, 9);
        if sum(gamesperslot(series_set)) <= 9
            continue
        end
        for j=setdiff(1:nteams, CALI_TEAMS)
            rin = rin + 1;
            for jj = CALI_TEAMS
                Ain(rin, (i  -1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = 2;
                if i-1 >= 1
                    if gamesperslot(i) == 3
                        Ain(rin, (i-1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -1;
                    else
                        Ain(rin, (i-1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -2;
                    end
                end
                if i+1 <= nslots
                    if gamesperslot(i) == 3
                        Ain(rin, (i+1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -1;
                    else
                        Ain(rin, (i+1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -2;
                    end
                end
            end
            bin(rin,1)=0;
            TRin(rin,:)=[9 i j inf];
            if whichconstraints(2,9)==1
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=whichconstraints(3,9);
            end
        end
    end

    % If flying out of California, need to play at least 2 series
    for j=CALI_TEAMS
        for i=1:nslots
            rin = rin + 1;
            for jj=setdiff(1:nteams, CALI_TEAMS)
                Ain(rin, (i  -1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = 2;
                if i-1 >= 1
                    if gamesperslot(i) == 3
                        Ain(rin, (i-1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -1;
                    else
                        Ain(rin, (i-1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -2;
                    end
                end
                if i+1 <= nslots
                    if gamesperslot(i) == 3
                        Ain(rin, (i+1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -1;
                    else
                        Ain(rin, (i+1-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1) = -2;
                    end
                end
            end
            bin(rin) = 0;
            TRin(rin,:)=[9 i j inf];
            if whichconstraints(2,9)==1
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=  whichconstraints(3,9);
            end
        end
    end

    % Can only travel to California once
    NOT_CALI=setdiff(1:nteams, CALI_TEAMS);
    for j=NOT_CALI
        cali_slack = zeros(1,nslots-1);
        for i=1:nslots-1
            rin = rin + 1;
            if i==1 % Count trip to Cali at start of season
                Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(CALI_TEAMS-1)+1) = 2;
            else
                Ain(rin, (i-1)*nteams*nteams+(j-1)*nteams+(NOT_CALI-1)+1) = 1;
                Ain(rin, (i+1-1)*nteams*nteams+(j-1)*nteams+(CALI_TEAMS-1)+1) = 1;
            end
            bin(rin) = 1;
            TRin(rin,:) = [9.1 i j inf];
            cols=cols+1;
            Ain(rin,cols)=-1;
            cali_slack(i) = cols;
        end

        rin = rin + 1;
        Ain(rin, setdiff(cali_slack,0)) = 1;
        bin(rin) = 1;
        TRin(rin,:) = [9 inf j inf];
        if whichconstraints(2,9)==1
            cols=cols+1;
            Ain(rin,cols)=-1;
            f(cols,1)= 10*whichconstraints(3,9);
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C10; Homestand and Roadtrip Control
if whichconstraints(1,10)==1
    % 2 week limit on roadtrips and homestands
    for i = 1:nslots
        % Find set of series to restrict, build until length greater
        % than 2 weeks
        series_set = get_series_set(i, 12);
        if sum(gamesperslot(series_set)) <= 12
            continue
        elseif sum(gamesperslot(series_set(2:end))) > 12
            continue  % will be handled in next iteration
        end
        for j=1:nteams
            % At most 12 games at home i.e. no long home stands
            % Handles all cases, even with 3 game series
            rin = rin + 1;
            for s = series_set
                Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=gamesperslot(s);
            end
            bin(rin) = 12;
            TRin(rin, :) = [10 i j 1];
            if whichconstraints(2,10)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-10;
                f(cols,1)= whichconstraints(3,10);
            end

            % At least 1 game at home i.e. no long road trips
            rin = rin + 1;
            for s = series_set
                Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(s);
            end
            bin(rin) = -1;
            TRin(rin, :) = [10 i j -1];
            if whichconstraints(2,10)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-10;
                f(cols,1)= whichconstraints(3,10);
            end
        end
    end

    % 3 Week limit for all teams, no slack
    for j=1:nteams
        for i = 1:nslots
            series_set = get_series_set(i,18);
            if sum(gamesperslot(series_set)) <= 18
                continue
            end

            % At most 18 games at home i.e. no long home stands
            rin = rin + 1;
            for s = series_set
                Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=gamesperslot(s);
            end
            bin(rin) = 18;
            TRin(rin, :) = [10 i j 3];
            
            % At most 1 games at home i.e. no long road trips
            rin = rin + 1;
            for s = series_set
                Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(s);
            end
            bin(rin) = -1;
            TRin(rin, :) = [10 i j -3];
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C11; of every five consecutive series, we want 2 or 3 home;
if whichconstraints(1,11)==1
    for i=1:nslots-4
        for j=setdiff(1:nteams, CALI_TEAMS)
            rin=rin+1;
            for ell=0:4 % for each consecutive 5 series
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1; % team j play at home
            end
            bin(rin,1)=3; % for <= 3 series
            TRin(rin,:)=[11 i j inf];
            if whichconstraints(2,11)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                
            end
            rin=rin+1;
            for ell=0:4 % for each consecutive 5 series
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1; % team j play at home
            end
            bin(rin,1)=-2; % for >= 2 series
            TRin(rin,:)=[11 i j inf];
            if whichconstraints(2,11)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C12; no pair of teams should play each other more than four times in 5 series
% Except Cali Teams
if whichconstraints(1,12)==1
    for i=1:(nslots-4)
        for j=setdiff(1:nteams, CALI_TEAMS)
            for jj=setdiff((j+1):nteams, CALI_TEAMS) % for 2 different teams
                rin=rin+1;
                for ell=0:4 % for consecutive 5 series
                    Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    Ain(rin,(i+ell-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=1;
                end
                bin(rin,1)=2; % play <= 2 series
                TRin(rin,:)=[12 i j jj];
                if whichconstraints(2,12)==1 % compute slack*penalty
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,12);
                    
                end
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C13; every three consecutive weekends, each team, number home weekends is one or two
if whichconstraints(1,13)==1
    for z=1:(nltemplate-1)-2
        i=template(z:z+2,6); % Counting Saturdays
        for j=setdiff(1:nteams, CALI_TEAMS)
            rin=rin+1;
            for ell=1:3
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-1;
            TRin(rin,:)=[13 i(1) j inf];
            if whichconstraints(2,13)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,13);
                
            end

            rin=rin+1;
            for ell=1:3
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=2;
            TRin(rin,:)=[13 i(1) j inf];
            if whichconstraints(2,13)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,13);
                
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C14; of every five consecutive weekends, each team, number of weekends is two or three
if whichconstraints(1,14)==1
    for z=1:(nltemplate-1)-4
        i =template(z:z+4,5); % Fridays
        for j=setdiff(1:nteams, CALI_TEAMS)
            rin=rin+1;
            for ell=1:5 % for each consecutive 5 series
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1; % team j play at home
            end
            bin(rin,1)=-2; % for >= 2 series
            TRin(rin,:)=[14 i(1) j inf];
            if whichconstraints(2,14)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,14);
                
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,14);
                
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,14);
                
            end
            rin=rin+1;
            for ell=1:5 % for each consecutive 5 series
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1; % team j play at home
            end
            bin(rin,1)=3;  % for 3 series
            TRin(rin,:)=[14 i(1) j inf];
            if whichconstraints(2,14)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,14);
                
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,14);
                
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,14);
                
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C15; of every seven consecutive weekends, each team, number of weekends is three or four
if whichconstraints(1,15)==1
    for z=1:(nltemplate-1)-6
        i=template(z:z+6,6); %counting Saturdays
        for j=setdiff(1:nteams, CALI_TEAMS)
            rin=rin+1;
            for ell=1:7
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-3;
            TRin(rin,:)=[15 i(1) j inf];
            if whichconstraints(2,15)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,15);
                
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,15);
                
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,15);
                
            end
            rin=rin+1;
            for ell=1:7
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=4;
            TRin(rin,:)=[15 i(1) j inf];
            if whichconstraints(2,15)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,15);
                
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,15);
                
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,15);
                
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C16; max 12 games between any teams
if whichconstraints(1,16)==1
    % Cannot see same team more than 12 times in any 21-game span
    for i=1:nslots
        series_set = get_series_set(i,21);
        if sum(gamesperslot(series_set)) <=21
            continue
        end
        for j=1:nteams
            for jp=setdiff(1:nteams,j)
                rin = rin +1;
                for s=series_set
                    Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=gamesperslot(s);
                    Ain(rin,(s-1)*nteams*nteams+(jp-1)*nteams+(j-1)+1)=gamesperslot(s);
                end
                bin(rin) = 12;
                TRin(rin,:)=[16 i j jp];
                if whichconstraints(2,3)==1
                    for qq=1:whichconstraints(4,3)
                        cols=cols+1;
                        Ain(rin,cols)=-3 * qq;
                        f(cols,1)=qq*whichconstraints(3,3);
                    end
                end
            end
        end
    end

    % Can't play back to back series if off day in middle of series
    for i=1:nslots-1
        for j=1:nteams
            for jp=setdiff(1:nteams,j)
                if ismember(i,breaksafter)
                    rin=rin+1;
                    Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1;
                    Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1;
                    bin(rin,1)=1;
                    TRin(rin,:)=[16 i j jp];
                    if whichconstraints(2,3)==1
                        for qq=1:whichconstraints(4,3)
                            cols=cols+1;
                            Ain(rin,cols)=-qq;
                            f(cols,1)=qq*whichconstraints(3,3);
                        end
                    end
                end
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C17; no revisiting a stadium until w intervening series pass
if whichconstraints(1,17)==1
    w=2;                   % how many intervening slots
    for i=1:nslots-w
        for j=1:nteams
            for jp=1:nteams
                if j~=jp % 2 different teams
                    rin=rin+1;
                    for ip=i:i+w % during intervening series
                        Ain(rin,(ip-1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1; % the 2 teams can meet
                    end
                    bin(rin,1)=1; % for at most once
                    TRin(rin,:)=[17 i j jp];
                    if whichconstraints(2,17)==1 % compute slack*penalty
                        cols=cols+1;
                        Ain(rin,cols)=-1;
                        f(cols,1)=  whichconstraints(3,17);
                        
                    end
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C18; no traveling over a rest day
if whichconstraints(1,18)==1
    for i=breaksafter % for break after series
        for j=1:nteams
            rin=rin+1;
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;  % team j play consecutive 2 series at home?
            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            bin(rin,1)=-1; % for at most once
            TRin(rin,:)=[18 i j inf];
            if whichconstraints(2,18)==1 % compute slack*penalty
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,18);
                
            end
        end
    end
end
completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C19 Soft constrained days   
if whichconstraints(1,19)==1
    for i=1:nslots 
        for j=1:nteams 
            if SOFTWIRE(i,j)==-100 %if team requests to be away on that series
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                    
                end
            elseif SOFTWIRE(i,j)==-101 %home and playing division
                rin=rin+1;
                for jj=1:nteams
                    if divn(j)~=divn(jj)
                        Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    end
                end
                bin(rin,1)=0;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                end
            elseif SOFTWIRE(i,j)==-102 %away and in division
                rin=rin+1;
                for jj=1:nteams
                    if divn(j)~=divn(jj) 
                        Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    end
                end
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                end
            elseif SOFTWIRE(i,j)>0 % visit specific stadium
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(SOFTWIRE(i,j)-1)+1)=-1;
                bin(rin,1)=-1;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C20 HARDWIRE
if whichconstraints(1,20)==1
    for i=1:nslots
        for j=1:nteams
            if HARDWIRE(i,j)==-100
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    
                end
            end
            if HARDWIRE(i,j)==-101
                rin=rin+1;
                for jj=1:nteams
                    if divn(j)~=divn(jj)
                        Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    end
                end
                bin(rin,1)=0;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    
                end
            end
            if HARDWIRE(i,j)==-102
                rin=rin+1;
                for jj=1:nteams
                    if divn(j)~=divn(jj)
                        Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    end
                end
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    
                end
            end
            if HARDWIRE(i,j)>0
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(HARDWIRE(i,j)-1)+1)=-1;
                bin(rin,1)=-1;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


%% C21: control 12-game homestands
if whichconstraints(1,21)==1
    locaslack=zeros(nslots-1,nteams); % zeroth matrix with(nslot-1) by nteams
    for i=1:nslots-1 % counter number of 12-game homestands
        series_set = get_series_set(i,11);
        if sum(gamesperslot(series_set)) <= 11
            continue
        end
        for j=1:nteams
            rin=rin+1;
            for s=series_set
                Ain(rin,(s-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=gamesperslot(s);
            end
            bin(rin,1)=11; % should not have more than 11 consecutive games at home
            TRin(rin,:)=[21.1 i j inf];
            cols=cols+1;
            Ain(rin,cols)=-1;
            locaslack(i,j)=cols; % keep track of slack variable columns
        end
    end

    % at most one 12-game homestand during the season per team
    for j=1:nteams
        rin=rin+1;
        for i=1:(nslots-1) % loop through all slots
            % skip empty slack variables
            if locaslack(i,j) > 0
                Ain(rin,locaslack(i,j))=1;
            end
        end
        bin(rin,1)=1; % only one 12-game homestand allowed per team
        TRin(rin,:)=[21 inf j inf];
        if whichconstraints(2,21)==1
            cols=cols+1;
            Ain(rin,cols)=-1;
            f(cols,1)=whichconstraints(3,21); % regular penalty for 2 homestands
            cols=cols+1;
            Ain(rin,cols)=-1;
            f(cols,1)=100*whichconstraints(3,21); % higher penalty for 3 homestands
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%% C22: max 12 games between any two teams
if whichconstraints(1,22)
    for j=1:nteams
        for jj=j+1:nteams
            rin = rin + 1;
            for i=1:nslots
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=gamesperslot(i);
                Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=gamesperslot(i);
            end
            % if all(ismember([j jj], CALI_TEAMS))
            %     bin(rin)=30; % Cali teams can play each other 30 times
            % else
                bin(rin,1)=27; 
            % end
            TRin(rin,:)=[22 inf j jj];
            if whichconstraints(2,22)==1
                for qq=1:whichconstraints(4,22)
                    cols=cols+1;
                    Ain(rin,cols)=-3*qq;
                    f(cols,1)=qq*whichconstraints(3,22); % full penalty
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C23: each team has to play everyone else in their group at least 18 times home or away
if whichconstraints(1,23) == 1
    % iterate through each group
    for group = {GROUP_1, GROUP_2, GROUP_3, GROUP_4}
        teams_in_group = group{:};
        for j = teams_in_group
            for jj = setdiff(teams_in_group, j)
                rin = rin + 1;
                for i = 1:nslots
                    Ain(rin, (i-1)*nteams*nteams + (j-1)*nteams + (jj-1) + 1) = -gamesperslot(i);
                    Ain(rin, (i-1)*nteams*nteams + (jj-1)*nteams + (j-1) + 1) = -gamesperslot(i);
                end
                bin(rin,1) = -18;  %each team plays every other team at least 18 times
                TRin(rin,:) = [23 inf j jj];
                
                if whichconstraints(2,23) == 1
                    for qq = 1:whichconstraints(4,23) % compute slack*penalty
                        cols = cols + 1;
                        Ain(rin,cols) = -qq;
                        f(cols,1) = qq * whichconstraints(3,23);
                    end
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Gurobi Setup

for i=completed:-1:2
    howmanyeq(i)=howmanyeq(i)-howmanyeq(i-1);
    howmanyineq(i)=howmanyineq(i)-howmanyineq(i-1);
    howmanycolumns(i)=howmanycolumns(i)-howmanycolumns(i-1);
end
howmanycolumns(1)=howmanycolumns(1)-nslots*nteams*nteams;

manyeq=howmanyeq
manyineq=howmanyineq
manycolumns=howmanycolumns
numberofvariables=cols

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Aeq=Aeq(1:req,1:cols);
Ain=Ain(1:rin,1:cols);
beq=beq(1:req,1);
bin=bin(1:rin,1);
f=f(1:cols,1);

TRin=TRin(1:rin,:);
TReq=TReq(1:req,:);

%%%% x=bintprog(f,Ain,bin,Aeq,beq);    for MATLAB use

newA = [Aeq; Ain];
newb = [beq; bin];
newA = sparse(newA);
sense_eq = repmat('=',req,1);
sense_in = repmat('<',rin,1);
sense = [sense_eq; sense_in];
vtype = repmat('B',cols,1);
model.A = newA;
model.obj = f;
model.modelsense = 'min';
model.rhs = newb;
model.sense = sense;
model.vtype = vtype;
if length(xstart)>2
    rr=nslots*nteams*nteams;
    xstart=[xstart(1:rr); NaN(cols-rr,1)];
	model.start = xstart;
end
mkdir(directory);
params.Logfile = [directory '/logfile' num2str(runnum) '.txt'];
params.Threads=80;
params.MIPFocus=1;
params.Method = 3;
params
result = gurobi(model,params);

%% Get Results
%If infeasible, try to find which constraints are causing issues
if strcmp(result.status,'INFEASIBLE')
    const_key = zeros(req+rin,1);
    counter = 1;
    for c = 1:completed
        const_key(counter:counter+manyeq(c)) = c;
        counter = counter+manyeq(c);
    end
    for c = 1:completed
        const_key(counter:counter+manyineq(c)) = c;
        counter = counter+manyineq(c);
    end
    disp("Possible Infeasible Constraints:");
    iis = gurobi_iis(model,params);
    disp(const_key(iis.Arows==1));
    ForExcel = [];
    nprobs = [];
    SchedInfo = [];
    problist = [];
    return
end

result.status
x = result.x;
x=round(x);
csvwrite([directory '/x.csv'], x);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SchedInfo=zeros(nslots,nteams);
ByTeam=zeros(nslots,nteams);
ByStadium=zeros(nslots,nteams);
for i=1:nslots
    for j=1:nteams
        for k=1:nteams
            if x((i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)==1
                ByTeam(i,j)=k;
                if j~=k
                    ByStadium(i,k)=j;
                    SchedInfo(i,j)=k;
                    SchedInfo(i,k)=-j;
                end
            end
        end
    end
end

AinTruncate=Ain(:, 1:(nslots*nteams*nteams)  );
AeqTruncate=Aeq(:, 1:(nslots*nteams*nteams)  );
xTruncate=x( 1:(nslots*nteams*nteams)   );
probin=AinTruncate*xTruncate > bin;
probeq=AeqTruncate*xTruncate ~= beq;
% remove constraints that only rely on combinations of slack variables, since 
% they will automatically show up in the problist after truncating Ain, Aeq
probinlist=TRin(setdiff(find(probin), find(all(AinTruncate==0,2))),:); 
probeqlist=TReq(setdiff(find(probeq), find(all(AeqTruncate==0,2))),:); 

% Get errors for rows that worked with only slack variables
onlyzerorows=find(all(Ain(:,1:(nslots*nteams*nteams))==0,2));
[row,col] = find(Ain(onlyzerorows,:)<0);
violatedslacks = onlyzerorows(row(find(x(col))));

probinlist = [probinlist;TRin(violatedslacks,:)];
problist=[probinlist; probeqlist];
problist=problist(mod(problist(:,1), 1) == 0, :);

nprobs=zeros(1,completed);
for i=1:completed
    nprobs(i)=sum( problist(:,1) == i );
end

ForExcel = zeros(nslots+length(breaksafter),1+nteams);
for i = 1:nslots
    row = i+ sum(i>breaksafter);
    ForExcel(row,:) = [ByTeam(i,:) gamesperslot(i)];
end
for i = 1:length(ForExcel)
    if sum(ForExcel(i,:))==0
        ForExcel(i,nteams+1) = 1;
    end
end

% Calculate Air and Bus mileage
team_mileage = zeros(2,nteams);
for j = 1:nteams
    for i = 1:nslots-1
        if SchedInfo(i,j) < 0
            from = j;
        else
            from = SchedInfo(i,j);
        end
        if SchedInfo(i+1,j) < 0
            to = j;
        else
            to = SchedInfo(i+1,j);
        end
        % Bus Mileage on first row, air on second
        if sum([ismember(from, CALI_TEAMS), ismember(to, CALI_TEAMS)]) == 1 && ~ismember(BOI, [from to])
            % "air" if "to" or "from" is in California, not both and not neither
            % Not air if going to or from Boise
            team_mileage(2, j) = team_mileage(2, j) + Dist(from, to);
        else
            team_mileage(1, j) = team_mileage(1, j) + Dist(from, to);
        end
    end
end

% Calculate Matchups
matchups = zeros(nteams);
for i = 1:nslots
    for j = 1:nteams
        opp = abs(SchedInfo(i,j));
        matchups(j, opp) = matchups(j, opp) + gamesperslot(i);
    end
end

start = [directory '/PIONEER25_' num2str(runnum)];
csvwrite([start '_Sched.csv'], SchedInfo);
csvwrite([start '_ForExcel.csv'], ForExcel);
csvwrite([start '_nprobs.csv'], nprobs);
csvwrite([start '_problist.csv'], problist);
csvwrite([start '_mileage.csv'], team_mileage);
reportname = [start '_report.txt'];

PIONEERreportgenerator(reportname, problist, SchedInfo, template)

end

