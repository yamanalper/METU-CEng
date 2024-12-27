% CHANGES DONE TO THIS FILE WILL BE OVERWRITTEN. 
% IT WILL HAVE NO EFFECT ON YOUR GRADE.

:- module(kb, [blogger/1, reader/2, posted/2, follow/2, blogpost/4, alreadyread/2]).

% Problem Definition

% Your millennial friend is yapping about the old web again, 
% how good it was, how everybody had their own blogs, and all that. 
% He is currently using an RSS feed to follow his remaining favourite bloggers. 
% You, being a child of the internet with algorithmic feeds, decide to implement 
% some extended functionality to your friend's preferred way of reading blogs. 
% To achieve this, you will implement some Prolog methods that focus on 
% blogger and post recommendations.


% blogger(bloggernick).
blogger(crimeow). 
blogger(internetbaby).
blogger(tashrants). 
blogger(basicape).
blogger(beyazfutbolfan).

% reader(readernick, listofinteresttopics).
reader(tijocat, [cyberCrimes, popCulture, fashion, science, food]).
reader(justaguy, [cryptocurrency, finance, sports]).

% posted(bloggernick, postid).
posted(crimeow, pid001).
posted(crimeow, pid002).
posted(internetbaby, pid021).
posted(internetbaby, pid022).
posted(basicape, pid031).
posted(beyazfutbolfan, pid032).
posted(tashrants, pid041).
posted(tashrants, pid042).

% follows(readernick, bloggernick).
follows(tijocat,crimeow).
follows(tijocat,internetbaby).
follows(justaguy, basicape).

% blogpost(postid, shortpostname, topic, date).
blogpost(pid001, spywaredump, cyberCrimes, 20230922).
blogpost(pid002, spywaredumpagain, cyberCrimes, 20231028).
blogpost(pid021, metgala2024flops, fashion, 20240510).
blogpost(pid022, celebdivorcewatch, celebGossip, 20240116).
blogpost(pid031, getricheasynow, cryptocurrency, 20240313).
blogpost(pid032, amanhocamdur, sports, 20210317).
blogpost(pid041, culturewrap2023, popCulture, 20231231).
blogpost(pid042, futureofacademia, science, 20221231).

% alreadyread(readernick, postid).
alreadyread(tijocat, pid001).
alreadyread(tijocat, pid021).
alreadyread(justaguy, pid031).