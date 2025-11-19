#include <iostream>
#include "monitor.h"
enum ItemType { AAA, BBB, CCC };

/* You can define any global variable
   function and class definition here.
   Programs will first call initStore() then
   all other functions
*/

class Store : public Monitor{
   int cap[3], avail[3], reserved[3], maxOrder;
   Condition canBuy, canSupply;

   public:
   Store() : canBuy(this), canSupply(this){}
   
   void initStore(int cA, int cB, int cC, int mO) {
      __synchronized__;
      cap[0] = cA;
      cap[1] = cB;
      cap[2] = cC;
      maxOrder = mO;
      avail[0] = cA;
      avail[1] = cB;
      avail[2] = cC;
      reserved[0] = 0;
      reserved[1] = 0;
      reserved[2] = 0;
   }
   
   void buy(int aA, int aB, int aC) {
      __synchronized__;
      while(aA > maxOrder || aB > maxOrder || aC > maxOrder || aA > avail[0] || aB > avail[1] || aC > avail[2]){
         canBuy.wait();
      }
      avail[0] -= aA;
      avail[1] -= aB;
      avail[2] -= aC;
      canSupply.notifyAll();
   }
   
   void maysupply(int itype, int n) {
      __synchronized__;
      while(avail[itype] + n + reserved[itype] > cap[itype]){
         canSupply.wait();
      }
      reserved[itype] += n;
   }
   
   void supply(int itype, int n) {
      __synchronized__;
      reserved[itype] -= n;
      avail[itype] += n;
      canBuy.notifyAll();
   }
   
   void monitorStore(int c[3], int a[3]) {
      __synchronized__;
      for(int i = 0; i < 3; i++){
         c[i] = cap[i];
      }
      for(int i = 0; i < 3; i++){
         a[i] = avail[i];
      }
   }
};

Store gStore;

void initStore(int cA, int cB, int cC, int mO) {
    gStore.initStore(cA, cB, cC, mO);
}

void buy(int aA, int aB, int aC) {
    gStore.buy(aA, aB, aC);
}

void maysupply(int itype, int n) {
    gStore.maysupply(itype, n);
}

void supply(int itype, int n) {
    gStore.supply(itype, n);
}

void monitorStore(int c[3], int a[3]) {
    gStore.monitorStore(c, a);
}
