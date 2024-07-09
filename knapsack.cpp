#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Product {
  string name;
  string category;
  int category_id;
  double price;
};

void displayProduct(const Product& product)
{
  cout << "Name: " << product.name << endl;
  cout << "Category: " << product.category << endl;
  cout << "Category ID: " << product.category_id << endl;
  cout << "Price: $" << product.price << endl;
  cout << endl;
}

// Function to solve the knapsack problem
int calculateKnapsack(const vector<Product>& products, int bagSize, double budget, vector<vector<int>>& dp) {
  int n = products.size();
  int budgetInt = static_cast<int>(budget * 100); // Convert budget to integer cents

  // Initialize the dp table with zeros
  dp.assign(n + 1, vector<int>(budgetInt + 1, 0));

  for (int i = 0; i <= n; ++i) {
    for (int w = 0; w <= budgetInt; ++w) {
      if (i == 0 || w == 0) {
        dp[i][w] = 0; // Base case: no items or no budget
      } else {
        int productPriceInt = static_cast<int>(products[i - 1].price * 100);
        if (productPriceInt > w) {
          dp[i][w] = dp[i - 1][w]; // Cannot include the current product
        } else {
          dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - productPriceInt] + productPriceInt); // Maximize value
        }
      }
    }
  }
}


// Function to reconstruct the solution from DP table
vector<Product> reconstructSolution(const vector<Product>& products, const vector<vector<int>>& dp, double budget) {
  int n = products.size();
  int budgetInt = static_cast<int>(budget * 100);
  vector<Product> selectedProducts;

  for (int i = n; i > 0 && budgetInt > 0; --i)
  {
    int productPriceInt = static_cast<int>(products[i - 1].price * 100);
    if (dp[i][budgetInt] != dp[i - 1][budgetInt]) {
      selectedProducts.push_back(products[i - 1]);
      budgetInt -= productPriceInt;
    }
  }

  return selectedProducts;
}

int main()
{
  vector<Product> products =
  {
    {"Shirt", "Clothing", 1, 20.0},
    {"Pants", "Clothing", 1, 30.0},
    {"Book", "Education", 2, 15.0},
    {"Pen", "Stationery", 3, 5.0},
    {"Laptop", "Electronics", 4, 500.0},
  };

  cout << "Available Products:\n";
  for (int i = 0; i < products.size(); i++)
  {
    displayProduct(products[i]);
  }
  int bagSize;
  double budget;
  int categoryId;

  cout << "\nEnter your bag size (weight limit): ";
  cin >> bagSize;

  cout << "Enter your budget: ";
  cin >> budget;

  cout << "Enter the category ID you're interested in (0 for all): ";
  cin >> categoryId;

  // Filter products based on category
 vector<Product> filteredProducts;
if (categoryId > 0) {
    for (size_t i = 0; i < products.size(); ++i) { // size_t= unsigned data type
        const Product& product = products[i];
        if (product.category_id == categoryId) {
            filteredProducts.push_back(product);
        }
    }
} else {
    filteredProducts = products; // If categoryId is 0 or less, include all products
}

  // DP table for knapsack
   vector<vector<int>> dp;

  // Solve knapsack problem
  int maxValue = calculateKnapsack(filteredProducts, bagSize, budget, dp);
  cout << "\nMaximum value of products you can buy: $" << static_cast<double>(maxValue) / 100.0 << endl; // static_cast is a type of casting operator

  // Reconstruct solution and display suggested products
  vector<Product> suggestedProducts = reconstructSolution(filteredProducts, dp, budget);
  if (!suggestedProducts.empty()) {
    cout << "\nSuggested Products:\n";
    for (const Product& product : suggestedProducts) {
      displayProduct(product);
    }
  } else {
    cout << "No products fit your criteria within the bag size and budget." << endl;
  }

  return 0;
}
