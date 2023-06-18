# Uber_Project_FMI

This is a university project for the Object-Oriented course in FMI, Sofia.
The goal of the project is to make a simplified system for taxi drivers and clients using the good OOP practices.

Some details about the code itself:
The clients and drivers share many common characteristics so that is why I made a parent class User.
The class Uber is the class in which is stored all the data about the orders and users.
The class Executer is responsible for the user interface. It also directly links the orders to the clients and the drivers.
