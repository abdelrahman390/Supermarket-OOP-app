#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>

using namespace std;
/*
abstract: product
---- shippable > interface
---- expirable > expaireMonth
*/

string weightCheck(int weight){
    if(weight > 1000){
        return to_string(weight / 1000) + "kg";
    } else {
        return to_string(weight) + "g";
    }
}

// Abstract class
class Product {
    string name;
    float price;
    float quantity;
    protected:
        float weight;
    
    public :
        Product(string _name, float _price, float _quantity, float _weight)
        {
            name = _name;
            price = _price;
            quantity = _quantity;
            weight = _weight;
        }

        virtual void viewDetail(){
            // cout << "--------------------------" << endl;
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            cout << "Quantity: " << quantity << endl;
            cout << "Weight: " << weight << endl;
        }

        bool isAvailable(float _quantity){
            if(_quantity <= quantity){
                return 1;
            } else {
                return 0;
            }
        }

        float getPrice(){
            return price;
        }

        string getName(){
            return name;
        }

        void removeQuantity(float qnt){
            quantity -= qnt;
        }
};

class shippable : public virtual Product  {
    public :
        shippable(string _name, float _price, float _quantity, float _weight)
        : Product(_name, _price, _quantity, _weight){}

        float getWeight(){
            return weight;
        }
    
};

class expirable : public virtual Product   {
    int expaireMonth;

    public :
        expirable(string _name, float _price, float _quantity, float _weight, int _expaireMonth)
        : Product(_name, _price, _quantity, _weight){
            expaireMonth = _expaireMonth;
        }

        bool isExpired(){
            time_t now = time(nullptr);     // current time
            tm* localTime = localtime(&now);

            int monthNumber = localTime->tm_mon + 1;
            if(expaireMonth > monthNumber){
                return 0;
            }
            return 1;
        }

        virtual void viewDetail(){
            Product::viewDetail();
            cout << "ExpaireMonth: " << expaireMonth << endl;
            cout << "isExpired: " << (isExpired() ? "Yes" : "No") << endl;
        }

        int getExpaireMonth(){
            return expaireMonth;
        }
};

class shippableAndexpirable: public shippable, public expirable{
    
    public:
        shippableAndexpirable(string _name, float _price, float _quantity , float _weight, int _expaireMonth): 
        Product(_name, _price, _quantity, _weight),
        shippable( _name, _price, _quantity, _weight),
        expirable( _name, _price, _quantity,_weight, _expaireMonth){}

        void viewDetail()
        {
            expirable::viewDetail();
        }
};

class market {
    vector<Product*> products;

    public:
        market(){
            cout << "\nInter 0 for not shippable or expirable products, 1 for shippable products, 2 for expirable products, and 3 for Shippable and Expirable products, then other product data" << endl;
        }

        Product* addProduct(int ch, string _name, float _price, float _quantity, float _weight = 0, int _expaireMonth = 0){
            Product* p = NULL;
            switch (ch) {
            case 0:
                p = new Product(_name, _price, _quantity, _weight);
                products.push_back(p);
                return p;
            case 1:
                if(_weight == 0){
                    cout << "Invalid weight value" << endl;
                } else {
                    p = new shippable(_name, _price, _quantity, _weight);
                    products.push_back(p);
                }
                return p;
            case 2:
                if(_expaireMonth == 0){
                    cout << "Invalid expaireMonth value" << endl;
                } else {
                    p = new expirable(_name, _price, _quantity, _weight, _expaireMonth);
                    products.push_back(p);
                }
                return p;
            case 3:
                if(_weight == 0 || _expaireMonth == 0){
                    cout << "Invalid weight or expaireMonth value" << endl;
                } else {
                    p = new shippableAndexpirable(_name, _price, _quantity, _weight, _expaireMonth);
                    products.push_back(p);
                }
                return p;
            default:
                cout << "Invalud product type choice" << endl;
                return p;
            }
        }

};

class Cart{
    vector<pair<Product*, float>> addedProducts;
    vector<pair<shippable *, float>> shippableProducts;

    public:
        bool thereAreshippable;
        float totalPrice = 0;

        void add(Product *product, float quntity)
        {
            bool added = 1;
            if (product->isAvailable(quntity))
            {
                // Check if shippable
                if(dynamic_cast<shippable*>(product)){
                    thereAreshippable = 1;
                    shippableProducts.push_back({dynamic_cast<shippable*>(product), quntity});
                    // Check if shippableAndexpirable
                } else if(dynamic_cast<shippableAndexpirable*>(product)){
                    // If shippableAndexpirable is not expired.
                    if(!(dynamic_cast<shippableAndexpirable*>(product)->isExpired())){
                        thereAreshippable = 1;
                        shippableProducts.push_back({dynamic_cast<shippable*>(product), quntity});
                        // If shippableAndexpirable is expired.
                    } else {
                        added = 0;
                        throw invalid_argument("This product is expired.");
                    }
                }
                if(added){
                    addedProducts.push_back({product, quntity});
                    product->removeQuantity(quntity);
                    totalPrice += (product->getPrice() * quntity);
                }
            }
            else
            {
                throw invalid_argument("There is not enough quantity in the stock.");
            }
        }

        vector<pair<Product*, float>> getProductList(){
            return addedProducts;
        }

        vector<pair<shippable*, float>> getshippableProductsList(){
            return shippableProducts;
        }

        bool isImpty() {
            if(addedProducts.size() > 0){
                return 0;
            } else {
                return 1;
            }
        }

};

class customer{
public:
    string name;
    float balance;
    customer(string _name, float _balance){
            name = _name;
            balance = _balance;
        }
};

class ShippingSecvice{
    float totalWight = 0;

public:
    ShippingSecvice(vector<pair<shippable *, float>> _shippableProducts)
    {
        shipProducts(_shippableProducts);
    }

    void shipProducts(vector<pair<shippable *, float>> shippableProducts){
        cout << "** Shipment notice **" << endl;
        for(auto& p: shippableProducts){
            cout << p.second << "X " << p.first->getName() << "    " << weightCheck(p.first->getWeight() * p.second * 1000) << endl;
            totalWight += p.first->getWeight() * p.second ;
        }
        cout << "Total package weight " << weightCheck(totalWight) << "\n\n";
    }

};

void checkoutReceipt(vector<pair<Product*, float>> products, Cart* cart){
    float total = 0;
    cout << "** Checkout receipt **" << endl;
    for(auto& p: products){
        cout << p.second << "X " << p.first->getName() << "    " << p.first->getPrice() * p.second << "$" << endl;
        total += p.first->getPrice() * p.second ;
    }
    cout << "------------------------" << "\n";
    cout << "Subtotal      " << total << "$" << "\n";
    if(cart->thereAreshippable){
        cout << "Shipping      " << 30 << "$"  << "\n";
        total += 30;
    }
    cout << "Amount        " << total << "$"  << "\n";
}
void checkout(customer* customer, Cart* cart) {
    vector<pair<Product*, float>> products = cart->getProductList();
    vector<pair<shippable *, float>> shippableProducts = cart->getshippableProductsList();

    if(cart->isImpty()){
        throw invalid_argument("Cart is empty.");
    }
    
    if(customer->balance < cart->totalPrice){
        throw invalid_argument("insufficient balance!");
    }

    if(cart->thereAreshippable){
        ShippingSecvice s(shippableProducts);
    }

    checkoutReceipt(products, cart);
}

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
    cout << "-----------------"<< endl;
    tv->viewDetail();
    cout << "-----------------"<< endl;
    Sushi->viewDetail();
    cout << "-----------------"<< endl;
    cheese->viewDetail();
    cout << "-----------------"<< endl;
    // ****************** print products details ******************

    Cart cart;
    cart.add(mobileScratch, 4);
    cart.add(tv, 1);

    // Name: abdelrahman, balance: 2000
    customer abdelrahman("Abdelrahman", 2000);

    checkout(&abdelrahman, &cart);

    return 0;
}
