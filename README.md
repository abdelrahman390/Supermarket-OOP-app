## A Supermarket Object-Oriented Programming app 
                                                 ┌──────────────────────┐
                                                 │      Product         │  (base class)
                                                 │ - name: string       │
                                                 │ - price: float       │
                                                 │ - quantity: float    │
                                                 │ # weight: float      │
                                                 │ + viewDetail()       │
                                                 │ + isAvailable(q)     │
                                                 │ + getPrice()         │
                                                 │ + getName()          │
                                                 │ + removeQuantity(q)  │
                                                 └─────────┬────────────┘
                                                           │ (virtual inheritance)
                                            ┌──────────────┴──────────────┐
                                            │                             │
                                    ┌───────┴────────┐           ┌────────┴────────┐
                                    │   shippable    │           │   expirable     │
                      (Interface)   │(inherits v-    │           │(inherits v-     │
                                    │  Product)      │           │  Product)       │
                                    │ + getWeight()  │           │ - expaireMonth  │
                                    │                │           │ + isExpired()   │
                                    └───────┬────────┘           │ + viewDetail()  │
                                            │                    └────────┬────────┘
                                            │                             │
                                            └───────────┬─────────────────┘
                                                        │
                                         ┌──────────────┴──────────────┐
                                         │ shippableAndexpirable       │  (multiple inheritance)
                                         │ inherits shippable,expirable│
                                         │ + viewDetail()              │
                                         └─────────────────────────────┘

Other cooperating classes (not part of product inheritance):
 - market          : manages product creation & container(vector<Product*>)
 - Cart            : holds vector<pair<Product*, quantity>> and vector<pair<shippable*, quantity>>
 - customer        : holds name & balance
 - ShippingSecvice : computes & prints shipment / total weight
 - free functions  : weightCheck(), checkoutReceipt(), checkout()

```cpp
int main(){
    market myMarket;

    // normal product, price: 30, quntity: 5, weight: 2.
    Product* mobileScratch = myMarket.addProduct(0, "mobileScratch", 30, 5, 2); // normal product

    // shippable product, price: 1000, quntity: 15, weight: 20.
    Product* tv = myMarket.addProduct(1, "Tv", 1000, 15, 20); // shippable product

    // Expirble product, price: 2, quntity: 10, weight: 1, expaireMonth: 10. 
    Product* Sushi = myMarket.addProduct(2, "Sushi", 5, 20, 2, 12); // expirble product 

    // shippable and expirble product, price: 2, quntity: 10, weight: 1, expaireMonth: 10. 
    Product* cheese = myMarket.addProduct(3, "cheese", 2, 10, 1, 10); // shippable and expirble product 

    mobileScratch->viewDetail();
      // Name: mobileScratch
      // Price: 30
      // Quantity: 5
      // Weight: 2
    cout << "-----------------"<< endl;
    tv->viewDetail();
      // Name: Tv
      // Price: 1000
      // Quantity: 15
      // Weight: 20
    cout << "-----------------"<< endl;
    Sushi->viewDetail();
      // Name: Sushi
      // Price: 5
      // Quantity: 20
      // Weight: 2
      // ExpaireMonth: 12
      // isExpired: No
    cout << "-----------------"<< endl;
    cheese->viewDetail();
      // Name: cheese
      // Price: 2
      // Quantity: 10
      // Weight: 1
      // ExpaireMonth: 10
      // isExpired: No
    cout << "-----------------"<< endl;
    // ****************** print products details ******************

    Cart cart;
    cart.add(mobileScratch, 4);
    cart.add(tv, 1);

    // Name: abdelrahman, balance: 2000
    customer abdelrahman("Abdelrahman", 2000);

    checkout(&abdelrahman, &cart);
      //Output:
        // ** Shipment notice **
        // 1X Tv    20kg
        // Total package weight 20g
        
        // ** Checkout receipt **
        // 4X mobileScratch    120$
        // 1X Tv    1000$
        // ------------------------
        // Subtotal      1120$
        // Shipping      30$
        // Amount        1150$

    return 0;
}

```
