from rest_framework.serializers import ModelSerializer, CharField
from . import models
from products.models import Product

class ProductSerializer(ModelSerializer):
    category = CharField(source='get_category_display')
    class Meta:
        model = Product
        fields = ['id', 'title', 'category', 'price', 'quantity']

class CartEntrySerializer(ModelSerializer):
    product = ProductSerializer()
    class Meta:
        model = models.CartEntry
        fields = ['product','quantity']

class CartSerializer(ModelSerializer):
    products = CartEntrySerializer(source='cartentry_set', many = True)
    class Meta:
        model = models.Cart
        fields = ['id', 'user', 'products', 'created_at', 'total_cost']

