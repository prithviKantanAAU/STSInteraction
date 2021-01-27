clc
clear
close all

filename_Log = 'MP Log.csv';

fid_Log = fopen(filename_Log);
data_header = textscan(fid_Log,'%s%s%s%s%s%s%s%s%s%s','Delimiter',',');

col1 = string(data_header{1});      ori_Trunk_AP = double(col1(3:2:end));
col2 = string(data_header{2});      ori_Thigh_AP = double(col2(3:2:end));
col3 = string(data_header{3});      ori_Shank_AP = double(col3(3:2:end));
col4 = string(data_header{4});      ori_Trunk_ML = double(col4(3:2:end));
col5 = string(data_header{5});      ang_Hip = double(col5(3:2:end));
col6 = string(data_header{6});      ang_Knee = double(col6(3:2:end));
col7 = string(data_header{7});      angVel_Knee = double(col7(3:2:end));
col8 = string(data_header{8});      angVel_Hip = double(col8(3:2:end));
col9 = string(data_header{9});      stsPhase = double(col9(3:2:end));

fs = 100.0;
timeTotal = 1/fs * length(ori_Trunk_AP);
timeVec = 1/fs : 1/fs : timeTotal;

plot (timeVec,ori_Trunk_AP)
hold on
plot (timeVec,ori_Thigh_AP)
hold on
plot (timeVec,stsPhase*10)