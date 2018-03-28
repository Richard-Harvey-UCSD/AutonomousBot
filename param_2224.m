%Run this program to prepare Simulink model

%Distance between wheels
Dist_wheels=0.12;    %m

%DC motor model for a Faulhaber 2224SR009 IE2-16
R=8.71;         %Resistance Ohm
L=130e-6;       %Inductance H
Km=11.12e-3;    %Torque Nm/A
Ke=1.115e-3*60/(2*pi);%EMF constant V/rads
Jrot=2.7e-7;    %Rotor inertia Kg*m2
Brot=0;         %Rotor friction
Jwheel=4.013e-6;%Wheel inertia Kg*m2
Bwheel=0;       %Output gear friction
ro=0.9;         %Gear ratio performance
n=8;            %Gear ratio
Rwheel=0.065/2; %m
rads2rpm=60/(2*pi);
rads2mps=Rwheel;
e_ticks=16;