function [ForExcel,SchedInfo,x,problist,nprobs,teamtravel]=ScheduleSL2019()

addpath('/usr/local/gurobi/linux64/matlab')

xstart=[];

whichconstraints=[
%1 2  3    4    5    6   7 8  9   10   11   12   13   14   15   16   17    18  19   20  
 1 1  1    1    1    1   1 1  1    1    1    1    1    0    1    1    1     1   1    1  %which
 0 0  1    1    1    1   0 0  1    0    1    1    1    1    1    1    1     1   1    0  %is slack
 0 0 10^8 10^8 10^8 10^6 0 0 10^6 10^8 10^10 10^6 10^6 10^5 10^5 10^6 10^6 5000 500  10^8  %penalty
 0 0  4    2    2    0   0 0  3    0    0    0    0    0    0    0    0     0   0    0 ]; %howmuch slack

 
% C1; every team plays in every slot
% C2; every game is home vs visitor
% C3; every team has 70 home games
% C4; every team has home 22 fridays+saturdays, and Th between 10 and 12
% C5; distribution of games between teams
% C6; travel distance controls
% C7  every team home for one of first two slots, last two slots
% C8; each team is home for at least one of july3 and july4
% C9; control 10 game homestands
% C10; no team is home three consecutive series or away; no three-window violations.
% C11; of every five consecutive series, we want 2 or 3 home;
% C12; no pair of teams should play each other more than twice in 5 series
% C13; controlling 10 game roadtrips
% C14; of every five consecutive weekends, each team, number of weekends is two or three
% C15; July3 to July4 and weekend distance control
% C16; no back to back series between two teams
% C17; no revisiting a stadium until w intervening series pass
% C18; no traveling over a rest day
% C19 Hard constrained days  
% C20 Absolutely Hard constrained days 




template=  [% M  T  W  T  F  S  S    for 2019
              0  0  0  1  1  1  1  % week of Apr 1
              1  0  2  2  2  2  2  % week of Apr 8
              3  3  3  3  3  4  4  % week of Apr 15
              4  4  4  5  5  5  5  % week of Apr 22
              5  0  6  6  6  6  6  % week of May 29
              7  7  7  7  7  8  8  % week of May 6
              8  8  8  9  9  9  9  % week of May 13
              9  0  10 10 10 10 10 % week of May 20
              0  11 11 11 11 11 12 % week of May 27
              12 12 12 12 13 13 13 % week of Jun 3
              13 13 14 14 14 14 14 % week of Jun 10
              0  0  0  15 15 15 15 % week of Jun 17
              15 16 16 16 16 17 17 % week of Jun 24
              17 17 17 18 18 18 18 % week of Jul 1
              18 18 0  19 19 19 19 % week of Jul 8  
              19 19 0  20 20 20 20 % week of Jul 15
              21 21 21 21 21 22 22 % week of Jul 22
              22 22 22 23 23 23 23 % week of Jul 29 
              23 0  24 24 24 24 24 % week of Aug 5
               0 25 25 25 25 25 26 % week of Aug 12
              26 26 26 26 27 27 27 % week of Aug 19
              27 27 0  28 28 28 28 % week of Aug 26
              28 0  0  0  0  0  0  % week of Sep 2
              ];
[nltemplate,~]=size(template);
july3=16;
breaksafter=[1 5 9 13 14 18 19 23 24 27];
half=14;
shorttravel=[july3 14 16 25];

NORTH=    [1   2   3   4   5  ];
SOUTH=    [6   7   8   9   10 ];

%	  1    2     3     4     5     6     7     8     9     10    
%    JXN  CHA   TNS   MAD   BIR   MIS   BLX   MTG   PNS   JAX 
Dist=[
      0   252   323   173   226   295   418   315   445   713
    252     0   132   110   150   380   458   233   397   464
    323   132     0   239   279   509   587   361   525   529
    173   110   239     0    94   323   408   182   346   537
    226   105   279    94     0   233   315    89   252   433
    295   380   509   323   233     0   160   244   245   591
    418   458   587   408   315   160     0   227   120   465
    315   233   361   182   89    244   227     0   160   370
    445   397   525   346   253   245   120   160     0   360
    713   464   529   537   433   591   465   370   360     0 ]; 

%  In Hardwire, number is which stadium visiting
%  -100 means away from home
%  -101 means within division
%  -102 means away from home but within division
%	   1    2    3    4    5    6    7    8    9   10    
%     JXN  CHA  TNS  MAD  BIR  MIS  BLX  MTG  PNS  JAX 
HARDWIRE=[
       0    0    0    0    0    0    0    0    0    0    %1
       0    0    0    0    0    0    0    0    0    0    %2    
       0    0    0    0    0    0    0    0    0    0    %3  
       0    0    0    0    0  -100    0    0    0    0   %4   
       0    0    0    0    0    0    0    0    0    0    %5
       0    0    0    0    0    0    0    0    0    0    %6    
       0    0    0    0    0    0    0    0    0    0    %7  
       0    0    0    0    0    0    0    0    0    0    %8 
       0    0    0    0   -100  0  -100   0    0    0    %9 
       0    0    0    0    0    0    0    0    0    0    %10    
       0    0    0    0    0    0    0    0    0    0    %11
       0    0    0    0    0    0    0    0    0    0    %12   
       0    0    0    0    0    0    0    0    0    0    %13
       0    0    0    0    0    0  -100   0    0    0    %14    
       0    0    0    0    0    0    0    0    0    0    %15  
       0    0    0    0    0    0    0    0    0    0    %16   
       0    0    0    0    0    0    0    0    0    0    %17
     -100   0   -100  0    0    0    0    0  -100 -100   %18    
       0    0    0    0    0    0    0    0    0    0    %19  
       0    0    0    0    0    0    0    0    0    0    %20   
       0    0    0    0    0    0    0    0    0    0    %21
       0    0    0    0    0    0    0    0    0    0    %22    
       0    0    0    0    0    0    0    0    0    0    %23  
       0    0    0    0    0    0    0    0    0    0    %24   
       0    0    0    0    0    0    0    0    0    0    %25
       0    0    0    0    0    0    0    0    0    0    %26    
       0    0    0    0    0    0    0    0    0    0    %27 
       0    0    0    0    0    0    0   -100  0    0  ]; %28   
      
% if length(wheretoput)>1
%     for i=1:length(wheretoput) 
%         HARDWIRE(wheretoput(i),:)=PartSchedule(i,:);    
%     end 
% end
      
       
%  In Hardwire, number is which stadium visiting
%  -100 means away from home
%  -101 means within division
%  -102 means away from home but within division
%	   1    2    3    4    5    6    7    8    9   10    
%     JXN  CHA  TNS  MAD  BIR  MIS  BLX  MTG  PNS  JAX 
HARDWIREstronger=[
       0    0    0    0    0    0    0    0    0    0    %1
       0    0    0    0    0    0    0    0    0    0    %2    
       0    0    0    0    0    0    0    0    0    0    %3  
       0    0    0    0    0    0    0    0    0    0    %4   
       0    0  -100   0    0    0    0    0    0    0    %5
       0    0    0    0    0    0    0    0    0    0    %6    
       0    0    0    0    0    0    0    0    0    0    %7  
       0    0    0    0    0    0    0    0    0    0    %8 
       0    0    0    0    0  -100   0    0    0    0    %9
       0    0    0  -100 -100   0  -100   0    0  -100   %10    
       0    0    0    0    0    0    0    0    0    0    %11
       0    0    0    0    0    0    0    0    0    0    %12   
       0  -100   0    0    0    0    0    0    0    0    %13
       0  -100   0    0    0    0    0    0    0    0    %14    
       0    0    0    0    0    0    0    0    0    0    %15  
       0    0    0    0    0    0    0    0    0    0    %16   
       0    0    0    0    0    0    0  -100   0    0    %17
       0    0    0    0    0    0    0    0    0    0    %18    
       0    0    0    0    0    0    0    0    0    0    %19  
       0    0    0    0    0    0    0    0    0    0    %20   
       0    0    0    0    0    0    0    0    0    0    %21
       0    0    0    0    0    0    0    0    0    0    %22    
       0    0    0    0    0    0    0    0    0    0    %23  
     -100   0    0    0    0    0    0    0    0    0    %24   
       0    0    0    0    0    0    0    0    0    0    %25
       0    0    0    0    0    0    0    0    0    0    %26    
       0    0    0    0    0    0    0    0    0    0    %27 
       0    0    0    0    0    0    0    0  -100   0  ];%28   



nteams=max([NORTH SOUTH]);
nslots=max(max(template));
gamesperslot=zeros(1,nslots);
for i=1:nslots
    gamesperslot(i)=sum(sum(template==i));
end


numconstrainttypes=length(whichconstraints);
howmanyeq=zeros(1,numconstrainttypes);
howmanyineq=zeros(1,numconstrainttypes);
howmanycolumns=zeros(1,numconstrainttypes);


cushionrows=20000;
cushioncolumns=20000;

Ain=zeros(cushionrows,nslots*nteams*nteams+cushioncolumns);   % storing inequality constraints
Ain=sparse(Ain);
Aeq=zeros(cushionrows,nslots*nteams*nteams+cushioncolumns);   % storing equality constraints
Aeq=sparse(Aeq);
bin=zeros(cushionrows,1);
beq=zeros(cushionrows,1);
f=zeros(nslots*nteams*nteams+cushioncolumns,1);

TRin=zeros(cushionrows,4);  % will keep track of constrainttype, slot, team, stadium indices for the rows
TReq=zeros(cushionrows,4);
xslack=zeros(nslots*nteams*nteams+cushioncolumns,1); % xstart for slacks

rin=0;                                   % index of row for Ain
req=0;                                   % index of row for Aeq
cols=nslots*nteams*nteams;               % index of columns for Ain/Aeq 

% C1; every team plays in every slot
if whichconstraints(1,1)==1
    for i=1:nslots
        for j=1:nteams
            req=req+1;
            for k=1:nteams
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

% C2; every game is home vs visitor
if whichconstraints(1,2)==1
    for i=1:nslots
        for k=1:nteams
            req=req+1;
            for j=1:nteams
                if j==k
                    Aeq(req,(i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)=1;
                else
                    Aeq(req,(i-1)*nteams*nteams+(j-1)*nteams+(k-1)+1)=-1;
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

% C3; every team has 70 home games, 35 per half
if whichconstraints(1,3)==1
    for j=1:nteams
        rin=rin+1;
        for i=1:half
            Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(i);
        end
        bin(rin,1)=-35;
        TRin(rin,:)=[3 inf j inf];
        if whichconstraints(2,3)==1
            for qq=1:whichconstraints(4,3)
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=qq*whichconstraints(3,3);
                xslack(cols,1)=1;
            end
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=(half+1):nslots
            Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-gamesperslot(i);
        end
        bin(rin,1)=-35;
        TRin(rin,:)=[3 inf j inf];
        if whichconstraints(2,3)==1
            for qq=1:whichconstraints(4,3)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,3);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C4; every team has home 22 fridays+saturdays              
if whichconstraints(1,4)==1
   for j=1:nteams
        rin=rin+1;
        for z=1:(nltemplate-1)
            i=template(z,5);
            iii=template(z,6);
            if i==iii
                 Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-2;
            else
                 Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
                 Ain(rin,(iii-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
        end
        bin(rin,1)=-22  ;
        TRin(rin,:)=[4 inf j inf];
        if whichconstraints(2,4)==1
            for qq=1:whichconstraints(4,4)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,4);
                xslack(cols,1)=1;
            end
        end
        
        rin=rin+1;
        for z=1:(nltemplate-1)
            i=template(z,4);
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        end
        bin(rin,1)=-10;
        TRin(rin,:)=[4 inf j inf];
        if whichconstraints(2,4)==1
            for qq=1:whichconstraints(4,4)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,4);
                xslack(cols,1)=1;
            end
        end
        rin=rin+1;
        for z=1:(nltemplate-1)
            i=template(z,4);
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
        end
        bin(rin,1)=12;
        TRin(rin,:)=[4 inf j inf];
        if whichconstraints(2,4)==1
            for qq=1:whichconstraints(4,4)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,4);
                xslack(cols,1)=1;
            end
        end
        
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;


% C5; distribution of games between teams 
 
if whichconstraints(1,5)==1
    for j=NORTH
        for jj=NORTH
            if ~(j==jj)
                rin=rin+1;
                for i=1:nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                end
                bin(rin,1)=-2;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=SOUTH
        for jj=SOUTH
            if ~(j==jj)
                rin=rin+1;
                for i=1:nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                end
                bin(rin,1)=-2;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=NORTH
        for jj=NORTH
            if  jj>j
                rin=rin+1;
                for i=1:half
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-1;
                end
                bin(rin,1)=-1;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=SOUTH
        for jj=SOUTH
            if  jj>j
                rin=rin+1;
                for i=1:half
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-1;
                end
                bin(rin,1)=-1;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=NORTH
        for jj=NORTH
            if  jj>j
                rin=rin+1;
                for i=(half+1):nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-1;
                end
                bin(rin,1)=-1;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=SOUTH
        for jj=SOUTH
            if  jj>j
                rin=rin+1;
                for i=(half+1):nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-1;
                end
                bin(rin,1)=-1;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=NORTH
        for jj=SOUTH 
            rin=rin+1;
            for i=1:nslots
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=-1;
            end
            bin(rin,1)=-1;
            TRin(rin,:)=[5 i j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5)
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                    xslack(cols,1)=1;
                end
            end
            rin=rin+1;
            for i=1:nslots
                Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-1;
            TRin(rin,:)=[5 i j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5)
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                    xslack(cols,1)=1;
                end
            end
        end
    end
    for j=NORTH
        for jj=SOUTH
            rin=rin+1;
            for i=1:nslots
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=3;
            if (j==1)&&(jj==10)
                 bin(rin,1)=2;
            end
            TRin(rin,:)=[5 i j jj];
            if whichconstraints(2,5)==1
                for qq=1:whichconstraints(4,5)
                    cols=cols+1;
                    Ain(rin,cols)=-qq;
                    f(cols,1)=qq*whichconstraints(3,5);
                    xslack(cols,1)=1;
                end
            end
        end
    end 
    for j=NORTH
        for jj=NORTH
            if  jj>j
                rin=rin+1;
                for i=1:nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=1;
                end
                bin(rin,1)=5;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
                    end
                end
            end
        end
    end
    for j=SOUTH
        for jj=SOUTH
            if  jj>j
                rin=rin+1;
                for i=1:nslots
                    Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    Ain(rin,(i-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=1;
                end
                bin(rin,1)=5;
                TRin(rin,:)=[5 i j jj];
                if whichconstraints(2,5)==1
                    for qq=1:whichconstraints(4,5)
                        cols=cols+1;
                        Ain(rin,cols)=-qq;
                        f(cols,1)=qq*whichconstraints(3,5);
                        xslack(cols,1)=1;
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


% C6; travel distance controls

if whichconstraints(1,6)==1   
   for k=1:nteams
        for kk=1:nteams
            if (Dist(k,kk)>500)&&(Dist(k,kk)<535)
                for i=1:(nslots-1)
                    if sum(breaksafter==i)==0
                        for j=1:nteams
                            rin=rin+1;
                            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(k -1)+1)=1;
                            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(kk-1)+1)=1;
                            bin(rin,1)=1;
                            TRin(rin,:)=[6 i j k];
                            if whichconstraints(2,6)==1
                                cols=cols+1;
                                Ain(rin,cols)=-1;
                                f(cols,1)= .1*whichconstraints(3,6);
                                xslack(cols,1)=1;
                            end
                        end
                    end
                end
            end
            if (Dist(k,kk)>535)
                for i=1:(nslots-1)
                    if sum(breaksafter==i)==0
                        for j=1:nteams
                            rin=rin+1;
                            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(k -1)+1)=1;
                            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(kk-1)+1)=1;
                            bin(rin,1)=1;
                            TRin(rin,:)=[6 i j k];
                            if whichconstraints(2,6)==1
                                cols=cols+1;
                                Ain(rin,cols)=-1;
                                f(cols,1)=whichconstraints(3,6);
                                xslack(cols,1)=1;
                            end
                        end
                    end
                end
            end
        end
    end
    for j=2:9
        for i=breaksafter
             rin=rin+1;
             Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(10 -1)+1)=1;
             Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(1  -1)+1)=1;
             Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(1  -1)+1)=1;
             Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(10 -1)+1)=1;
             bin(rin,1)=1;
        end
    end
    
    rin=rin+1;
    for i=2:(half)
        Ain(rin,(i-1)*nteams*nteams+(1-1)*nteams+(10-1)+1)=1;
        Ain(rin,(i-1)*nteams*nteams+(10-1)*nteams+(1-1)+1)=1;
    end
    for i=(half+1):(nslots-1)
        Ain(rin,(i-1)*nteams*nteams+(1-1)*nteams+(10-1)+1)=1;
        Ain(rin,(i-1)*nteams*nteams+(10-1)*nteams+(1-1)+1)=1;
    end
    bin(rin,1)=0;    
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C7  
%     Every team is home first or second slot
%     Every team is home last or secondlast slot

if whichconstraints(1,7)==1
    for j=1:nteams           
        req=req+1;
        Aeq(req,(1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
        Aeq(req,(2-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)= 1;
        beq(req,1)=1;
        req=req+1;
        Aeq(req,(nslots-1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
        Aeq(req,(nslots  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
        beq(req,1)=1;
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C8; each team is home for at least one of july3 and july4, as well as
% before/after ASB
if whichconstraints(1,8)==1
    for j=1:nteams           
        rin=rin+1;
        Ain(rin,(july3  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        Ain(rin,(july3+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        bin(rin,1)=-1;
        TRin(rin,:)=[8 inf j inf];
        if whichconstraints(2,8)==1
            cols=cols+1;
            Ain(rin,cols)=-1;
            f(cols,1)=  whichconstraints(3,8);
            xslack(cols,1)=1;
        end
    end
    for j=1:nteams           
        rin=rin+1;
        Ain(rin,(half  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        Ain(rin,(half+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
        bin(rin,1)=-1;
        TRin(rin,:)=[8 inf j inf];
        if whichconstraints(2,8)==1
            cols=cols+1;
            Ain(rin,cols)=-1;
            f(cols,1)=  whichconstraints(3,8);
            xslack(cols,1)=1;
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C9; control 10-game homestands
if whichconstraints(1,9)==1
    locaslack=zeros(nslots-1,nteams);
    for i=1:nslots-1
        for j=1:nteams
            rin=rin+1;
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            bin(rin,1)=1;
            cols=cols+1;
            Ain(rin,cols)=-1;
            xslack(cols,1)=1;
            locaslack(i,j)=cols;
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=1:(nslots-1)
            Ain(rin,locaslack(i,j))=1;
        end
        bin(rin,1)=2;
        TRin(rin,:)=[9 inf j inf];
        if whichconstraints(2,9)==1
            for qq=1:whichconstraints(4,9)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,9);
                xslack(cols,1)=1;
            end
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=1:(half-1)
            Ain(rin,locaslack(i,j))=1;
        end
        bin(rin,1)=1;
        TRin(rin,:)=[9 inf j inf];
        if whichconstraints(2,9)==1
            for qq=1:whichconstraints(4,9)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=.2*qq*whichconstraints(3,9);
                xslack(cols,1)=1;
            end
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=(half+1):(nslots-1)
            Ain(rin,locaslack(i,j))=1;
        end
        bin(rin,1)=1;
        TRin(rin,:)=[9 inf j inf];
        if whichconstraints(2,9)==1
            for qq=1:whichconstraints(4,9)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=.2*qq*whichconstraints(3,9);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C10; no team is home three consecutive series or away; 
%            

if whichconstraints(1,10)==1  
    for i=1:nslots-2
        for j=1:nteams
            rin=rin+1;
            for ell=0:2
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=2;
            TRin(rin,:)=[10 i j inf];
            if whichconstraints(2,10)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,10);
                xslack(cols,1)=1;
            end
            rin=rin+1;
            for ell=0:2
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-1;
            TRin(rin,:)=[10 i j inf];
            if whichconstraints(2,10)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,10);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C11; of every five consecutive series, we want 2 or 3 home;
if whichconstraints(1,11)==1
    for i=1:nslots-4
        for j=1:nteams
            rin=rin+1;
            for ell=0:4
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=3;
            TRin(rin,:)=[11 i j inf];
            if whichconstraints(2,11)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                xslack(cols,1)=1;
            end
            rin=rin+1;
            for ell=0:4
                Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-2;
            TRin(rin,:)=[11 i j inf];
            if whichconstraints(2,11)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,11);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C12; no pair of teams should play each other more than twice in 5 series
if whichconstraints(1,12)==1 
    for i=1:(nslots-4)
        for j=1:nteams
            for jj=(j+1):nteams
                rin=rin+1;
                for ell=0:4
                    Ain(rin,(i+ell-1)*nteams*nteams+(j-1)*nteams+(jj-1)+1)=1;
                    Ain(rin,(i+ell-1)*nteams*nteams+(jj-1)*nteams+(j-1)+1)=1;
                end
                bin(rin,1)=2;
                TRin(rin,:)=[12 i j jj];
                if whichconstraints(2,12)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,12);
                    xslack(cols,1)=1;
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C13; controlling 10 game roadtrips
if whichconstraints(1,13)==1
    mocaslack=zeros(nslots-1,nteams);
    for i=1:nslots-1
        for j=1:nteams
            rin=rin+1;
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            bin(rin,1)=-1;
            cols=cols+1;
            Ain(rin,cols)=-1;
            xslack(cols,1)=1;
            mocaslack(i,j)=cols;
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=1:(nslots-1)
            Ain(rin,mocaslack(i,j))=1;
        end
        bin(rin,1)=2;
        TRin(rin,:)=[13 inf j inf];
        if whichconstraints(2,13)==1
            for qq=1:whichconstraints(4,13)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=qq*whichconstraints(3,13);
                xslack(cols,1)=1;
            end
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=1:(half-1)
            Ain(rin,mocaslack(i,j))=1;
        end
        bin(rin,1)=1;
        TRin(rin,:)=[13 inf j inf];
        if whichconstraints(2,13)==1
            for qq=1:whichconstraints(4,13)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=.2*qq*whichconstraints(3,13);
                xslack(cols,1)=1;
            end
        end
    end
    for j=1:nteams
        rin=rin+1;
        for i=(half+1):(nslots-1)
            Ain(rin,mocaslack(i,j))=1;
        end
        bin(rin,1)=1;
        TRin(rin,:)=[13 inf j inf];
        if whichconstraints(2,13)==1
            for qq=1:whichconstraints(4,13)
                cols=cols+1;
                Ain(rin,cols)=-qq;
                f(cols,1)=.2*qq*whichconstraints(3,13);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C14; of every five consecutive weekends, each team, number of weekends is two or three
if whichconstraints(1,14)==1
     for z=1:(nltemplate-1)-4
        i =template(z:z+4,5);
        for j=1:nteams
            rin=rin+1;
            for ell=1:5
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            end
            bin(rin,1)=-2;
            TRin(rin,:)=[14 i(1) j inf];
            if whichconstraints(2,14)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,14);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,14);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,14);
                xslack(cols,1)=1;
            end
            rin=rin+1;
            for ell=1:5
                Ain(rin,( i(ell)-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
            end
            bin(rin,1)=3;
            TRin(rin,:)=[14 i(1) j inf];
            if whichconstraints(2,14)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,14);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-2;
                f(cols,1)=2*whichconstraints(3,14);
                xslack(cols,1)=1;
                cols=cols+1;
                Ain(rin,cols)=-3;
                f(cols,1)=3*whichconstraints(3,14);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C15; No excessive travel Jul3/Jul4 and other
if whichconstraints(1,15)==1
    for k=1:nteams
        for kk=1:nteams
            if Dist(k,kk)>350
                for i=shorttravel
                    for j=1:nteams
                        rin=rin+1;
                        Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(k -1)+1)=1;
                        Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(kk-1)+1)=1;
                        bin(rin,1)=1;
                        TRin(rin,:)=[15 i j k];
                        if whichconstraints(2,15)==1
                            cols=cols+1;
                            Ain(rin,cols)=-1;
                            f(cols,1)= whichconstraints(3,15);
                            if Dist(k,kk)>500
                                f(cols,1)=10*whichconstraints(3,15);
                            end
                            xslack(cols,1)=1;
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

% C16; no back to back series between two teams
if whichconstraints(1,16)==1
    for i=1:nslots-1
        for j=1:nteams
            for jp=j+1:nteams
                rin=rin+1;
                Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1;
                Ain(rin,(i+1-1)*nteams*nteams+(jp-1)*nteams+(j-1)+1)=1;
                Ain(rin,(i  -1)*nteams*nteams+(jp-1)*nteams+(j-1)+1)=1;
                Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1;
                bin(rin,1)=1;
                TRin(rin,:)=[16 i j jp];
                if whichconstraints(2,16)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,16);
                    xslack(cols,1)=1;
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C17; no revisiting a stadium until w intervening series pass
if whichconstraints(1,17)==1
    w=2;                   % how many intervening slots
    for i=1:nslots-w
        for j=1:nteams
            for jp=1:nteams
                if j~=jp
                    rin=rin+1;
                    for ip=i:i+w
                        Ain(rin,(ip-1)*nteams*nteams+(j-1)*nteams+(jp-1)+1)=1;
                    end
                    bin(rin,1)=1;
                    TRin(rin,:)=[17 i j jp];
                    if whichconstraints(2,17)==1
                        cols=cols+1;
                        Ain(rin,cols)=-1;
                        f(cols,1)=  whichconstraints(3,17);
                        xslack(cols,1)=1;
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

% C18; no traveling over a rest day
if whichconstraints(1,18)==1
    for i=breaksafter
        for j=1:nteams
            rin=rin+1;
            Ain(rin,(i  -1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            Ain(rin,(i+1-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=-1;
            bin(rin,1)=-1;
            TRin(rin,:)=[18 i j inf];
            if whichconstraints(2,18)==1
                cols=cols+1;
                Ain(rin,cols)=-1;
                f(cols,1)=  whichconstraints(3,18);
                xslack(cols,1)=1;
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C19 Hard constrained days   

if whichconstraints(1,19)==1
    for i=1:nslots
        for j=1:nteams
            if HARDWIRE(i,j)==-100
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                    xslack(cols,1)=1;
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
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                    xslack(cols,1)=1;
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
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                    xslack(cols,1)=1;
                end
            end
            if HARDWIRE(i,j)>0
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(HARDWIRE(i,j)-1)+1)=-1;
                bin(rin,1)=-1;
                TRin(rin,:)=[19 i j inf];
                if whichconstraints(2,19)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,19);
                    xslack(cols,1)=1;
                end
            end
        end
    end
end

completed=completed+1
howmanyeq(completed)=req;
howmanyineq(completed)=rin;
howmanycolumns(completed)=cols;

% C20 HARDWIRE stronger

if whichconstraints(1,20)==1
    for i=1:nslots
        for j=1:nteams
            if HARDWIREstronger(i,j)==-100
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(j-1)+1)=1;
                bin(rin,1)=0;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    xslack(cols,1)=1;
                end
            end
            if HARDWIREstronger(i,j)==-101
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
                    xslack(cols,1)=1;
                end
            end
            if HARDWIREstronger(i,j)==-102
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
                    xslack(cols,1)=1;
                end
            end
            if HARDWIREstronger(i,j)>0
                rin=rin+1;
                Ain(rin,(i-1)*nteams*nteams+(j-1)*nteams+(HARDWIREstronger(i,j)-1)+1)=-1;
                bin(rin,1)=-1;
                TRin(rin,:)=[20 i j inf];
                if whichconstraints(2,20)==1
                    cols=cols+1;
                    Ain(rin,cols)=-1;
                    f(cols,1)=  whichconstraints(3,20);
                    xslack(cols,1)=1;
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
    xstart=[xstart(1:rr); xslack(rr+1:cols)];
    model.start = xstart;
end

params.Threads=150;
params.ConcurrentMIP=12;
params 
result = gurobi(model,params);

% result = gurobi(model);

result.status
x = result.x;
x=round(x);


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

Calendar=zeros(nltemplate,7,nteams);
for j=1:nteams
    for i=1:nltemplate
        for z=1:7
           if template(i,z)>0
               if ByTeam(template(i,z),j)~=j
                   Calendar(i,z,j)=ByTeam(template(i,z),j);
                   Calendar(i,z,ByTeam(template(i,z),j))=-j;
               end
           end
        end
    end
end

AinTruncate=Ain(:, 1:(nslots*nteams*nteams)  );
AeqTruncate=Aeq(:, 1:(nslots*nteams*nteams)  );
xTruncate=x( 1:(nslots*nteams*nteams)   );
probin=find(AinTruncate*xTruncate > bin );
probeq=find(AeqTruncate*xTruncate ~= beq );
probinlist=TRin(probin,:);
probeqlist=TReq(probeq,:);
problist=[probinlist; probeqlist];
nprobs=zeros(1,completed);
for i=1:completed
   nprobs(i)=sum( problist(:,1) == i ); 
end

ForExcel=[ByTeam gamesperslot'];
ForExcel=[ForExcel(1:               breaksafter(1),:)
          zeros(1,10)   1
          ForExcel(breaksafter(1)+1:breaksafter(2),:)
          zeros(1,10)   1
          ForExcel(breaksafter(2)+1:breaksafter(3),:)
          zeros(1,10)   1
          ForExcel(breaksafter(3)+1:breaksafter(4),:)
          zeros(1,10)   1
          ForExcel(breaksafter(4)+1:breaksafter(5),:)
          zeros(3,10)   ones(3,1)
          ForExcel(breaksafter(5)+1:breaksafter(6),:)
          zeros(1,10)   1
          ForExcel(breaksafter(6)+1:breaksafter(7),:)
          zeros(1,10)   1
          ForExcel(breaksafter(7)+1:breaksafter(8),:)
          zeros(1,10)   1
          ForExcel(breaksafter(8)+1:breaksafter(9),:)
          zeros(1,10)   1
          ForExcel(breaksafter(9)+1:breaksafter(10),:)
          zeros(1,10)   1
          ForExcel(breaksafter(10)+1:nslots,:)      ];


% % 
% the following teamtravel code should work but hasnt been 
% tested against our VBA displays
forcomp=[ 1:nteams ; ByTeam(1:half,:) ; 1:nteams ; ByTeam((half+1):nslots,:)  ; 1:nteams ];
teamtravel=zeros(1,nteams);
for i=1:(nslots+2)
    for j=1:nteams
       teamtravel(j)=teamtravel(j)+Dist( forcomp(i,j),forcomp(i+1,j)   ); 
    end
end
      
      

end






