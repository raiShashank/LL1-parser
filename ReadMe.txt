i will return  vector<set<int>>
I am given map<int, set<vector<int>>> infact in dis map terminals r also present in d form terminal -> terminal
convention terminals r mapped 2 -ve no. ,epsilon is also a terminal mapped to -1, -1000 is d mapping fr $ since it is also a terminal and dere is no need fr delimiter since diff productions fr same non-terminal r diff vectors
I need an array/vector of integers which r keys in ur map since I cannot iterate over ur map
I am also given d map conating d mapping i.e. non-terminal is mapped 2 which integer nd terminal is mapped 2 which integer

if x -> a b c d
I will check if first[a] is dere , if it does not exist, i will find first[a] recursively, since dere is no left-recursion hence
I won't encounter d case like
a -> m n o p
m -> x y z
in short jis non-terminal ka main first set construct kar raha hoon, only its first set is incomplete but all others whose first set is constructed , dey r final(not subject 2 change), der may b others whose first set is not yet constructed, dat's fine with me.
during recursion, i will never come back on d non-terminal whose first set is incomplete as left-recursion(direct as well as indirect) has alreday been removed.
I need an array/vector of integers which r keys in ur map since I cannot iterate over ur map
i will keep a data member which will keep all item numbers for which first sets r being recursively calculated at a time
if item is -ve i.e. it is a terminal, I hav not explicitly calculated its first_set, u can use it as a value on d fly