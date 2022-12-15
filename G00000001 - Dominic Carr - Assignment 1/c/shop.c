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

void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
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

struct Customer readCustomer(const char* file){
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
		char *cname = malloc(sizeof(char) * 50);
		strcpy(cname, cn);

		float budget = atof(strtok(NULL, ","));
		
		struct Customer list = { cname, budget };

		while ((read = getline(&line, &len, fp)) != -1) {
			char *n = strtok(line, ",");
			char *q = strtok(NULL, ",");
			int quant = atoi(q);

			char *p = malloc(sizeof(char) * 50);
			strcpy(p, n);
			struct Product product = {p};
			struct ProductStock ps = {product, quant};
			list.shoppingList[list.index++] = ps;

		}
		return list;
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

int main(void) 
{
	char section[] = "========================================================================================\n";
	struct Shop shop = createAndStockShop();
	char *file = "../customer.csv";
	struct Customer c = readCustomer(file);
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
			break;
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
		c = readCustomer(file);
		}
		else if (choice == '6')
		{
			printf(section);
			break;
		}
		else if (choice == '7')
		{
			printf(section);
			break;
		}
	}

    return 0;
}