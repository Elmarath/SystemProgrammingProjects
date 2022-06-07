usage: creates 7 threads (6 producer 1 distrubuter). Every producer needs 4 ingredients (S, U, F, G)
to create the main product. Also every producer has 2 of 4 ingredients infinite and these ingredients
are distinct from another. So distrubuter cycle throughs producers and their infinite ingredient and 
distrubutes the producers which are lack of that ingredient. So that producers can produce the main 
ingredient. 
When a producer starts to produce the main product. It locks the whole system. So there would not be 
a mismatch on ingredient count.
Also this "take-give" approach happens in the file named baklava.txt. So we can see which producer had
the curtain ingredient at any time.

 /filename