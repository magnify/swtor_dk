USER KARMA


This module will keep track of your users' karma using the fantastic VotingAPI. 
It does so by assigning one vote per user, where the vote is the user's karma.

This module was developed as part of Drigg (http://www.drigg-code.org); however,
it also stands on its own: it will work with anything that uses the VotingAPIs.


# What is "Karma"?

**NOTE: This module needs MySql 4.1.X or higher to work. (The queries to find 
out the users' rank are a little too much for MySql 4.0.X)**

The technicality
----------------

Karma in this module's context is the sum of all the karma received from
the hook hook_user_karma_partial.

OK, that wasn't very clear. Let me try it again.

The user_karma module asks other modules whether they have defined a way
to calculate karma. Those modules implemented hook_user_karma_partial().
The user's karma will be the sum of all those karma amounts.

This means that you need extra modules to *actually* count karma.

THe module comes with:

* user_karma_receive_vote_node
* user_karma_receive_vote_comment
* user_karma_give_vote 
* user_karma_write_comment (coming soon!)

The nice bit is that these plugins are very, very easy to write. So,
any reasonable feature request will be easily implemented!

How does karma work?
--------------------
So, if user 99 for example writes 10 comments, and they all get votes > 0,
the person's karma will stay positive. Vice versa, if user 98 has 10 comments
and they are all flamebaits which get votes < 0, the person's karma will be
negative.

This assumes that rating can be < 0.

Now... some modules work differently, and only allow positive voting (for
example rating from 1 to 10 etc.). This module will work in those cases
too: since karma expires (meaning that only content newer than N days will be
considered for karma), the users' karma will eventually stabilise. 

# Role assigning

This module will manage a set of roles for you, and will assign them according
to the users' karma. There are two ways to assign roles: based on the raw
karma amount, or based on the user ranking, out of 10000. This will give you
quite a bit of granularity.

# Views

This module uses the VotingAPI. This means that you can use it with the "views"
module.
All you need to know is that the module adds one single "vote" (or "entry") 
for each user, where the value_type is karma_points and the tag is "karma".
I used a different value_type because I didn't want the VotingAPI to calculate
sum(), average(), etc. (it doesn't make sense in this context).

I am not a big "views" user. Please let me know if you can use this module
easily.

