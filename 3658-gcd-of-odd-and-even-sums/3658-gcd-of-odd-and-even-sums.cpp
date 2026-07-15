class Solution {
public:
    int gcdOfOddEvenSums(int n) {
        int sume=0;
        int k=n,i=2;
        while(k--){
            sume+=i;
            i+=2;
            
        }
        int sum=0;
         i=1;
        while(n--){
            sum+=i;
            i+=2;
        }
        return gcd(sume,sum);
    }
};