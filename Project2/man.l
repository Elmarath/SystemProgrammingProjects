.TH 170419041_baklava.c 1
.SH NAME
170419041_baklava.c - This program simulates a system where 6 producers with ingredient needs, produces a product with a deliverer's help.
.SH SYNOPSIS
[InitializeChefs]
[InitializeDeliverer]
[PrintChefs]
[PrintDeliverer]
[PrintChef]
[WriteCurrentChefToFile]
[CookBaklava]
[Deliver]
[UpdateChefFromFile]
.SH DESCRIPTION
creates 7 threads (6 producer 1 distrubuter). Every producer needs 4 ingredients (S, U, F, G)
to create the main product. Also every producer has 2 of 4 ingredients infinite and these ingredients
are distinct from another. So distrubuter cycle throughs producers and their infinite ingredient and 
distrubutes the producers which are lack of that ingredient. So that producers can produce the main 
ingredient. 
When a producer starts to produce the main product. It locks the whole system. So there would not be 
a mismatch on ingredient count.
Also this "take-give" approach happens in the file named baklava.txt. So we can see which producer had
the curtain ingredient at any time.
.SH FUNCTIONS
.TP
.BR InitializeChefs
initializes the chefs with their infinite and current ingredients.
.TP
.BR InitializeDeliverer
initializes the deliverer's current ingredients.
.TP
.BR PrintChefs
prints the chefs's current ingredients from file.
.TP
.BR PrintChef
prints the specified chef's current ingredients.
.TP
.BR WriteCurrentChefToFile
writes the chef's current ingredients to the file.
.TP
.BR CookBaklava
Cooks baklava by using the chef's current ingredients.
.TP
.BR Deliver
delivers the ingredients to the chefs.
.TP
.BR UpdateChefFromFile
Updates the chef's current ingredients from the file.
