class Solution {
public:

    /*
        INTUITION:
        -----------
        Each equation represents a relationship between two variables.

        Example:
            a / b = 2

        This means:
            a -> b has weight 2
            b -> a has weight 1/2

        We can think of variables as nodes and equations as weighted edges.

        To find:
            x / y

        We need to find a path from x to y.

        The product of edge weights along that path gives the answer.

        Example:
            a / b = 2
            b / c = 3

            a -> b -> c

            a / c = 2 * 3 = 6
    */


    double dfs(string src, string dest,
               unordered_map<string, vector<pair<string, double>>> &graph,
               unordered_set<string> &vis) {

        // If source and destination are same:
        // x/x = 1 always
        if (src == dest)
            return 1.0;


        // Mark current node visited to avoid cycles
        vis.insert(src);


        // Explore all connected variables
        for (auto &nbr : graph[src]) {

            string next = nbr.first;
            double weight = nbr.second;


            // Avoid visiting already explored nodes
            if (!vis.count(next)) {

                double result = dfs(next, dest, graph, vis);


                // If destination was found,
                // multiply current edge value with remaining path value
                if (result != -1.0) {
                    return weight * result;
                }
            }
        }


        // No path exists from src to dest
        return -1.0;
    }


    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {


        /*
            BUILD GRAPH:

            Equation:
                a / b = 2

            becomes:

                a ---> b   (2)
                b ---> a   (1/2)

            We store both directions because queries
            can ask either a/b or b/a.
        */

        unordered_map<string, vector<pair<string, double>>> graph;


        for (int i = 0; i < equations.size(); i++) {

            string a = equations[i][0];
            string b = equations[i][1];

            graph[a].push_back({b, values[i]});

            graph[b].push_back({a, 1.0 / values[i]});
        }



        vector<double> ans;


        for (auto &query : queries) {

            string src = query[0];
            string dest = query[1];


            /*
                If either variable does not exist,
                no answer is possible.
            */
            if (!graph.count(src) || !graph.count(dest)) {
                ans.push_back(-1.0);
                continue;
            }


            unordered_set<string> vis;


            // DFS finds a path from src to dest
            ans.push_back(dfs(src, dest, graph, vis));
        }


        return ans;
    }
};


/*
=========================================================
MISTAKES IN PREVIOUS CODE:
=========================================================


1) Treating variables as characters instead of strings

Wrong:
    unordered_map<char, vector<pair<char,double>>> mp;


Problem:
    Variable names are complete strings.

    Example:
        "apple" / "banana"

    You cannot split them into:
        'a' -> 'a'
        'e' -> 'b'


Correct:
    unordered_map<string, vector<pair<string,double>>> graph;



---------------------------------------------------------


2) Wrong graph construction

You did:

    mp[x[0]].push_back({y[b-1], value});


Example:

    abc / xyz = 2


Your graph becomes:

    a -> z


But the actual relation is:

    "abc" -> "xyz"


Variables are nodes, not characters.



---------------------------------------------------------


3) Incorrect query handling

You did:

    if (x.length() != y.length())
        return -1;


This is wrong.

Example:

    a / apple

can be valid if a relation exists.

Length of variable names has no meaning.



---------------------------------------------------------


4) Multiplying all DFS paths

You had:

    ans = ans * edge * dfs(...)


Problem:

If a node has multiple neighbours,
you multiply answers from different paths.

DFS should return when the FIRST valid path is found.

Correct:

    if(result != -1)
        return edge * result;



---------------------------------------------------------


5) Using vector<bool>(26)

You assumed variables are:

    'a' to 'z'


But variables can be:

    "x"
    "abc"
    "temperature"


Therefore use:

    unordered_set<string> visited;


=========================================================
REVISION SUMMARY:

1. Make graph:
       variable -> (connected variable, ratio)

2. Add reverse edges:
       a/b = k
       b/a = 1/k

3. For every query:
       DFS from source to destination

4. Multiply weights along the path

5. If no path exists:
       return -1

=========================================================
*/