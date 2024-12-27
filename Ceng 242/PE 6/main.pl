:- module(main, [is_blogpost_written_by/2, has_post_in_topic/2, get_follower_count_of_blogger/2, get_post_count_of_topic/2, filter_posts_by_date/4, recommend_posts/2, recommend_bloggers/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

is_blogpost_written_by(BloggerNick, ShortPostName) :-
    posted(BloggerNick, PostID),
    blogpost(PostID, ShortPostName, _, _).

has_post_in_topic(BloggerNick, Topic) :-
    posted(BloggerNick, PostID),
    blogpost(PostID, _, Topic, _), !.
    
get_follower_count_of_blogger(BloggerNick, FollowerCount) :-
    findall(ReaderNick, follows(ReaderNick, BloggerNick), Followers),
    length(Followers, FollowerCount).
    
get_post_count_of_topic(Topic, PostCount) :-
    findall(PostID, blogpost(PostID, _, Topic, _), Posts),
    length(Posts, PostCount).

filter_posts_by_date([], _, _, []).

filter_posts_by_date([PostID|Rest], OldestDate, NewestDate, [PostID|Filtered]) :-
    blogpost(PostID, _, _, Date),
    Date >= OldestDate,
    Date =< NewestDate,
    filter_posts_by_date(Rest, OldestDate, NewestDate, Filtered).

filter_posts_by_date([PostID|Rest], OldestDate, NewestDate, Filtered) :-
    blogpost(PostID, _, _, Date),
    (Date < OldestDate ; Date > NewestDate),
    filter_posts_by_date(Rest, OldestDate, NewestDate, Filtered).

recommend_posts(ReaderNick, ListOfRecommendedPosts) :-
    reader(ReaderNick, InterestTopics),
    findall(PostID, (blogpost(PostID, _, Topic, _), member(Topic, InterestTopics)), AllPosts),
    findall(ReadPost, alreadyread(ReaderNick, ReadPost), ReadPosts),
    subtract(AllPosts, ReadPosts, ListOfRecommendedPosts).

recommend_bloggers(ReaderNick, ListOfRecommendedBloggers) :-
    reader(ReaderNick, InterestTopics),
    findall(BloggerNick, follows(ReaderNick, BloggerNick), FollowedBloggers),
    findall(BloggerNick, 
            (blogger(BloggerNick),
             posted(BloggerNick, PostID),
             blogpost(PostID, _, Topic, _),
             member(Topic, InterestTopics),
             \+ member(BloggerNick, FollowedBloggers)),
            PotentialBloggers),
    sort(PotentialBloggers, ListOfRecommendedBloggers).
