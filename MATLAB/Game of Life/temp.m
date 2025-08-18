%--------------------------PART-C----------------------------
%-------------Blinker and Beacon with edges------------------

%Initial matrix: blinker
blinker =[0 0 0 0 0; 0 0 1 0 0; 0 0 1 0 0; 0 0 1 0 0; 0 0 0 0 0];

%Output matrix containing final result 
%Needed because otherwise we will be altering the initial matrix as we go
output1 = zeros(5, 5);

%Loops through every element in the entire matrix
for i = 1:length(blinker)
    for j = 1:length(blinker)

        %Reset the sum back to zero for every element
        sum = 0;

        %Conditional statements to check each position (if there exists)
        %neighboring the center piece is a 1
        %Adds 1 to sum to keep track of total neighbors that is a 1
        if i ~= 1 && j ~= 1 && blinker(i-1, j-1) == 1
            sum = sum + 1;
        end
        if i ~= 1 && blinker(i-1, j) == 1
            sum = sum + 1;
        end
        if i ~= 1 && j ~= length(blinker) && blinker(i-1, j+1) == 1
            sum = sum + 1;
        end
        if j ~= 1 && blinker(i, j-1) == 1
            sum = sum + 1;
        end
        if j ~= length(blinker) && blinker(i, j+1) == 1
            sum = sum + 1;
        end
        if i ~= length(blinker) && j ~= 1 && blinker(i+1, j-1) == 1
            sum = sum + 1;
        end
        if i ~= length(blinker) && blinker(i+1, j) == 1
            sum = sum + 1;
        end
        if i ~= length(blinker) && j ~= length(blinker) && blinker(i+1, j+1) == 1
            sum = sum + 1;
        end

        %Conditional statements to check the four rules of the game
        %Updates output accordingly
        if blinker(i, j) == 1
            if sum < 2 || sum > 3
                output1(i, j) = 0;
            else 
                output1(i, j) = 1;
            end
        else
            if sum == 3
                output1(i, j) = 1;
            else
                output1(i, j) = 0;
            end
        end
    end
end

%Display the final output matrix to the screen
%disp(output1)
%subplot(2, 2, 1)
%imagesc(output1)
%colormap(gray)


%Initial matrix: beacon
beacon = [0 0 0 0 0 0; 0 1 1 0 0 0; 0 1 0 0 0 0; 0 0 0 0 1 0; 0 0 0 1 1 0; 0 0 0 0 0 0];

%Output matrix containing final result 
%Needed because otherwise we will be altering the initial matrix as we go
output2 = zeros(6, 6);

%Loops through every element in the entire matrix
for i = 1:length(beacon)
    for j = 1:length(beacon)

        %Reset the sum back to zero for every element
        sum = 0;

        %Conditional statements to check each position (if there exists)
        %neighboring the center piece is a 1
        %Adds 1 to sum to keep track of total neighbors that is a 1
        if i ~= 1 && j ~= 1 && beacon(i-1, j-1) == 1
            sum = sum + 1;
        end
        if i ~= 1 && beacon(i-1, j) == 1
            sum = sum + 1;
        end
        if i ~= 1 && j ~= length(beacon) && beacon(i-1, j+1) == 1
            sum = sum + 1;
        end
        if j ~= 1 && beacon(i, j-1) == 1
            sum = sum + 1;
        end
        if j ~= length(beacon) && beacon(i, j+1) == 1
            sum = sum + 1;
        end
        if i ~= length(beacon) && j ~= 1 && beacon(i+1, j-1) == 1
            sum = sum + 1;
        end
        if i ~= length(beacon) && beacon(i+1, j) == 1
            sum = sum + 1;
        end
        if i ~= length(beacon) && j ~= length(beacon) && beacon(i+1, j+1) == 1
            sum = sum + 1;
        end

        %Conditional statements to check the four rules of the game
        %Updates output accordingly
        if beacon(i, j) == 1
            if sum < 2 || sum > 3
                output2(i, j) = 0;
            else 
                output2(i, j) = 1;
            end
        else
            if sum == 3
                output2(i, j) = 1;
            else
                output2(i, j) = 0;
            end
        end
    end
end

%Display the final output matrix to the screen
%disp(output2)
%subplot(2, 2, 2)
%imagesc(output2)
%colormap(gray)


%--------------------------PART-D----------------------------
%-----------------Table with edges 12th gen------------------

%Initial matrix: table
table = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

%Find the dimensions of the initial matrix so that the code works for any
%matrix of size MxN 
[r, c] = size(table);

%Output matrix containing final result 
%Needed because otherwise we will be altering the initial matrix as we go
output3 = zeros(r, c);

%For loop that runs 12 times to find the result at the 12th generation
for n = 1:12
    %Loops through every element in the entire matrix
    for i = 1:r
        for j = 1:c
    
            %Reset the sum back to zero for every element
            sum = 0;
    
            %Conditional statements to check each position (if there exists)
            %neighboring the center piece is a 1
            %Adds 1 to sum to keep track of total neighbors that is a 1
            if i ~= 1 && j ~= 1 && table(i-1, j-1) == 1
                sum = sum + 1;
            end
            if i ~= 1 && table(i-1, j) == 1
                sum = sum + 1;
            end
            if i ~= 1 && j ~= c && table(i-1, j+1) == 1
                sum = sum + 1;
            end
            if j ~= 1 && table(i, j-1) == 1
                sum = sum + 1;
            end
            if j ~= c && table(i, j+1) == 1
                sum = sum + 1;
            end
            if i ~= r && j ~= 1 && table(i+1, j-1) == 1
                sum = sum + 1;
            end
            if i ~= r && table(i+1, j) == 1
                sum = sum + 1;
            end
            if i ~= r && j ~= c && table(i+1, j+1) == 1
                sum = sum + 1;
            end
    
            %Conditional statements to check the four rules of the game
            %Updates output accordingly
            if table(i, j) == 1
                if sum < 2 || sum > 3
                    output3(i, j) = 0;
                else 
                    output3(i, j) = 1;
                end
            else
                if sum == 3
                    output3(i, j) = 1;
                else
                    output3(i, j) = 0;
                end
            end
        end
    end
    %Update table to be the output after going throught 1 generation
    %so that when we compute the next generation it will be continued from
    %the previous generation and not its initial state
    table = output3;
end

%Display the final output matrix to the screen
%disp(output3)
%subplot(2, 2, 3)
imagesc(output3)
colormap(gray)
