#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

struct Shop_Cust {
	struct Shop shop;
	struct Customer cust;
};

void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %lf\n", p.name, p.price);
	printf("-------------\n");
}

void printCustomer(struct Customer c)
{
	printf("\nCUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf("-------------\n");
	for(int i = 0; i < c.index; i++)
	{
		printProduct(c.shoppingList[i].product);
		printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price; 
		printf("The cost to %s will be €%.2f\n", c.name, cost);
	}
}



struct Shop createAndStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	float cash = atof(line);
	// printf("cash in shop is %.2f\n", cash);
	
	struct Shop shop = { cash };

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s IS A LINE", line);
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
    }
	
	return shop;
}

void printShop(struct Shop s)
{
	printf("Shop has %.2f in cash\n", s.cash);
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n", s.stock[i].quantity);
	}
}

struct Shop_Cust buyitem(struct Customer customer, struct Shop shop, int single)
{
	int instock = 1;	
	int inshop = 1;
	float cost = 0;
	int error = 0;
	int x = 0;
	struct Shop_Cust shop_cust = {shop, customer};
	struct Customer temp;
	temp.name = customer.name;


	for(int item = 0; item < customer.index; item++){
		for(int i = 0; i < shop.index; i++){
			if(*customer.shoppingList[item].product.name == *shop.stock[i].product.name){
				inshop = 1;
				if (customer.shoppingList[item].quantity <= shop.stock[i].quantity){
					cost = customer.shoppingList[item].quantity * shop.stock[i].product.price;
					break;
				}
				else if(instock != 0){
					instock = 0;
					printf("%s NOT ENOUGH IN STOCK.\n", customer.shoppingList[item].product.name);
					break;
				}
			}
			else{
				inshop = 0;
			}

		}			
		if (inshop == 0){
			printf("%s IS NOT IN THIS SHOP.\n", customer.shoppingList[item].product.name);	
		}


	}
	if(cost > customer.budget)
	{
		printf("INSUFFICIENT FUNDS\n");
	}

	else if(instock == 1 && inshop == 1){
	printf("SUCCESSFUL PURCHASE\n");
	customer.budget = customer.budget - cost;
	temp.budget = customer.budget;
	shop.cash = shop.cash - cost;
	customer = temp;
	shop_cust.shop = shop;
	shop_cust.cust = customer;
	}
	return shop_cust;
}

struct ProductStock createProduct(char *p, int quant, struct Shop shop){
			double price = 0;
			struct Product product = {p};
			for(int i =0 ; i < shop.index; i++)
			{
				if(*shop.stock[i].product.name == *product.name)
				{
					price = shop.stock[i].product.price;
					break;
				}
			}
			product.price = price;
			struct ProductStock ps = {product, quant};
			return ps;

}

struct Customer readCustomer(const char* file, struct Shop shop){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	if (fp == NULL)
        {
		struct Customer list = {};
        return list;
		}
	else
		{
		read = getline(&line, &len, fp);
		char *cn = strtok(line, ",");
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, cn);

		float budget = atof(strtok(NULL, ","));
		
		struct Customer list = { name, budget };

		while ((read = getline(&line, &len, fp)) != -1) {
			char *n = strtok(line, ",");
			char *q = strtok(NULL, ",");
			int quant = atoi(q);
			
			char *p = malloc(sizeof(char) * 50);
			strcpy(p, n);			
			struct ProductStock ps = createProduct(p, quant, shop);
			
			list.shoppingList[list.index++] = ps;

		}
		return list;
	}
}



void updateCustomerfile(struct Customer customer, char* file){
	FILE *fpt;
	fpt = fopen(file, "w");
	fprintf(fpt,"%s, %f", customer.name, customer.budget);
	for(int i = 0; i < customer.index; i++){
		fprintf(fpt, "%s, %d", customer.shoppingList[i].product.name, customer.shoppingList[i].quantity);
	}
}
void updateShopfile(struct Shop shop){
	FILE *fpt;
	fpt = fopen("../stock.csv", "w");
	fprintf(fpt, "%f",  shop.cash);
	for(int i = 0; i < shop.index; i++){
		fprintf(fpt, "%s, %f, %d", shop.stock[i].product.name, shop.stock[i].product.price, shop.stock[i].quantity);
	}
}

int main(void) 
{
	char section[] = "\n========================================================================================\n";
	struct Shop shop = createAndStockShop();
	char *file = "../customer.csv";
	struct Customer c = readCustomer(file, shop);
	char choice;
	while(1){
		choice = '0';
		printf(section);
		printf("WELCOME %s\n1) Show Shop. \n2) Show Customer Shopping List.\n3) Buy Specific Item.\n4) Buy Items. \n5) Select Customer.\n6) Add to Shopping List. \n7) Exit", c.name);
		printf("\nENTER NUMBER FOR OPTION: ");
		scanf(" %c", &choice);

		if (choice == '1')
		{
			printf(section);
			printShop(shop);
		}
		else if (choice == '2')
		{
			printf(section);
			printCustomer(c);
		}
		else if (choice == '3')
		{
			printf(section);
			break;
		}
		else if (choice == '4')
		{
			printf(section);
			struct Shop_Cust shop_cust = buyitem(c, shop, 0);
			c = shop_cust.cust;
			shop = shop_cust.shop;
			updateCustomerfile(c, file);
			updateShopfile(shop);
		}
		else if (choice == '5')
		{
			printf(section);
			printf("1) John(Succesful Purchase)(DEFAULT)\n");
        	printf("2) Cantof(Insufficient Funds)\n");
        	printf("3) Mrs400Loaves (Insufficient Stock)\n");
			printf("Input Number of Which Customer you Wish to be: ");
			scanf(" %c", &choice);
			if (choice == '1'){
            	file = "../customer.csv";
			}
        	else if (choice == '2'){
            	file = "../MrCantofCoke.csv";
			}
        	else if (choice == '3'){
            	file = "../Mrs400loaves.csv";
			}
		c = readCustomer(file, shop);
		}
		else if (choice == '6')
		{
			char name[20];
			char str[30];
			int quant;
			printf(section);			
			printf(" \nProduct Name:");
			scanf(" %[^\n]", &name);		
			printf(" \nQuantity: ");
			scanf("%d", &quant);
			char q = atoc(quant);
			char p[30] = name + "," + q;
			strncat(str, name, 1);
			strncat(str, ",", 1);
			strncat(str, q, 1);


			for(int i=0; i>=c.index+1; i++){
				
	
				createProduct(name, shop)
				
			}
		}
		else if (choice == '7')
		{
			printf(section);
			break;
		}
	}

    return 0;
}