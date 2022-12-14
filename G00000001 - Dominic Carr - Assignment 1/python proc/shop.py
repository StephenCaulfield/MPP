from dataclasses import dataclass, field
from typing import List
import csv

choice = ""
section = '========================================================================================'

@dataclass
class Product:
    name: str
    price: float = 0.0

@dataclass 
class ProductStock:
    product: Product
    quantity: int

@dataclass 
class Shop:
    cash: float = 0.0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str = ""
    budget: float = 0.0
    shopping_list: List[ProductStock] = field(default_factory=list)

def create_and_stock_shop():
    s = Shop()
    with open('../stock.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        s.cash = float(first_row[0])
        for row in csv_reader:
            p = Product(row[0], float(row[1]))
            ps = ProductStock(p, float(row[2]))
            s.stock.append(ps)
            #print(ps)
    return s
    
def read_customer(file_path):
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        c = Customer(first_row[0], float(first_row[1]))
        for row in csv_reader:
            name = row[0]
            quantity = float(row[1])
            p = Product(name)
            ps = ProductStock(p, quantity)
            c.shopping_list.append(ps)
        return c 

def buy_items(customer, shop):
    instock = 1
    cost = 0
    error = 0
    x = 0
    for item in customer.shopping_list:  

        for i in shop.stock:

            if item.product.name == i.product.name:
                instock = 1

                if item.quantity <= i.quantity:
                    cost += item.quantity * i.product.price
                    shop.stock[x].quantity = shop.stock[x].quantity - item.quantity

                else:
                    print(f"{item.product.name} NOT ENOUGH IN STOCK.")
                    error = 1
                    break

                break

            else:
                instock = 0
            x+=1

        if instock == 0:
            print(f"{item.product.name} IS NOT IN THIS SHOP.")  
            error = 1

    cost = round(cost, 2)

    if cost > customer.budget:
        print('INSUFFICIENT FUNDS')
        error = 1
        
    if error == 1:
        print('INVALID PURCHASE')
        return customer, shop
    else:
        shop.cash = round(shop.cash + cost, 2)
        customer.budget = round(customer.budget - cost,2)
        print("SUCCESSFUL PURCHASE")
        print(f'COST: {cost}')
        return customer, shop

def update_shop(s):
        new_list = list()
        new_list.append([s.cash])
        for item in s.stock:
            new_list.append([item.product.name, item.product.price, item.quantity])

        new_shop = open('..\stock.csv', 'w', newline = '')
        csv_writer = csv.writer(new_shop)
        csv_writer.writerows(new_list)
        new_shop.close()

def print_product(p):
    print(f'\nPRODUCT NAME: {p.name} \nPRODUCT PRICE: {p.price}')

def print_customer(c):
    print(f'CUSTOMER NAME: {c.name} \nCUSTOMER BUDGET: {c.budget}')
    
    for item in c.shopping_list:
        print_product(item.product)
        
        print(f'{c.name} ORDERS {item.quantity} OF ABOVE PRODUCT')
        cost = item.quantity * item.product.price
        print(f'The cost to {c.name} will be €{cost}')
        
def print_shop(s):
    print(f'Shop has {s.cash} in cash')
    for item in s.stock:
        print_product(item.product)
        print(f'The Shop has {item.quantity} of the above')

c = read_customer("../customer.csv")

s = create_and_stock_shop()

while True:

    print(section)
    print("WELCOME", c.name, "\n1) Show Shop. \n2) Show Customer Shopping List.\n3) Buy Specific Item.\n4) Buy Items.\n5) Select Customer.\n6) Exit")

    choice = input("ENTER NUMBER FOR OPTION: ")

    choice = choice.strip()

    if (choice == '1'):
        print(section)
        print_shop(s)

    elif (choice == '2'):
        print(section)
        print_customer(c)

    elif (choice == '3'):
        print(section)
        p = Product(input('Product Name: '))
        quant = int(input('Quantity: '))
        ps = ProductStock(p, quant)
        c2 = c
        c2.shopping_list = []
        c2.shopping_list.append(ps)
        buy_items(c2, s)
        update_shop(s)

    elif (choice == '4'):
        print(section)
        c, s = buy_items(c, s)
        print(s.cash)
        update_shop(s)


    elif (choice == '5'):
        print(section)
        print('1) John(Succesful Purchase)(DEFAULT)')
        print('2) Cantof(Insufficient Funds)')
        print('3) Mrs400Loaves (Insufficient Stock)')
        read_c = input('Input Number of Which Customer you Wish to be: ')
        if (read_c == '1'):
            c = read_customer("../customer.csv")
        elif (read_c == '2'):
             c = read_customer("../MrCantofCoke.csv")
        elif (read_c == '3'):
             c = read_customer("../Mrs400loaves.csv")

    elif (choice == '6'):
        print(section)
        break

    else:
        print(section)
        print('INVALID OPTION, PLEASE ENTER A VALID NUMBER')