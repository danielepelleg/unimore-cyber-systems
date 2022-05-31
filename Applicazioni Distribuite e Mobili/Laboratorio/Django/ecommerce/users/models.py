from django.db import models
from django.contrib.auth.models import User
from products.models import Product

# Create your models here.

class Customer(models.Model):
    GENDERS = [('M', 'Male'), ('F', 'Female')]
    LOCATION = [('EUR', 'Europe'), ('USA', 'America'), ('ASIA', 'Asia')]

    user = models.OneToOneField(User, on_delete=models.CASCADE)
    gender = models.CharField(default='M', choices=GENDERS, max_length=1)
    location = models.CharField(max_length=30, choices=LOCATION ,blank=True)
    birth_date = models.DateField(null=True, blank=True)

    def __str__(self) -> str:
        return self.user.username

class Cart(models.Model):
    user = models.ForeignKey(Customer, on_delete=models.CASCADE)
    products = models.ManyToManyField(
        Product,
        through='CartEntry',
        through_fields=('cart', 'product'),
    )
    created_at = models.DateTimeField(auto_now=True)

    def pay(self):
        self.isPaid = True
        super().save()

    @property
    def total_cost(self):
        cost = 0
        for prod in self.products.all():
            current_cart = CartEntry.objects.get(product=prod)
            cost += (prod.price * current_cart.quantity)
        return cost

class CartEntry(models.Model):
    cart = models.ForeignKey(Cart, on_delete=models.CASCADE)
    product = models.ForeignKey(Product, on_delete=models.CASCADE)
    quantity = models.IntegerField(default=0)

    class Meta:
        unique_together = ('cart', 'product',)

    def buy(self, new_quantity: int):
        self.product.quantity -= new_quantity
        self.product.save()
        super().save()

class Order(models.Model):
    ORDER_STATUS = [('P', 'Problem'), ('E', 'Elaborating'), ('S', 'Sent'), ('H', 'Handed')]

    user = models.ForeignKey(Customer, on_delete=models.CASCADE)
    products = models.ManyToManyField(
        Product,
        through='OrderEntry',
        through_fields=('order', 'product')
    )
    total = models.DecimalField(default=0.00, max_digits=5, decimal_places=2)
    created_at = models.DateTimeField(auto_now=True)
    status = models.CharField(default='E', choices=ORDER_STATUS, max_length=1)

class OrderEntry(models.Model):
    order = models.ForeignKey(Order, on_delete=models.CASCADE)
    product = models.ForeignKey(Product, on_delete=models.CASCADE)
    quantity = models.IntegerField(default=1)

    class Meta:
        unique_together = ('order', 'product',)