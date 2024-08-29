clc;
clear;
clear all;

% User input for values
radius1 = input("Enter the first radius: ");
radius2 = input("Enter the second radius: ");
radius3 = input("Enter the third radius: ");

% Calculate areas and perimeters
[area1, area2, area3] = Area_function(radius1, radius2, radius3);
[perimeter1, perimeter2, perimeter3] = Perimeter_function(radius1, radius2, radius3);

% Prepare data for plotting
x = [area1, area2, area3];
y = [perimeter1, perimeter2, perimeter3];

% Plot the graph
plot(x, y);
xlabel('Area');
ylabel('Perimeter');
title('Area vs Perimeter');
