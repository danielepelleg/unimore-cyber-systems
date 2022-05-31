from django.http import HttpResponse
from django.views import generic
from django.contrib.auth.decorators import login_required
from django.views.decorators.http import require_POST
from .models import Product
from users.models import Cart, CartEntry, Customer

# Create your views here.

class ProductListView(generic.ListView):
    template_name = 'products/product_list'
    #context_object_name = "product_list"

    def get_queryset(self) -> list:
        return Product.objects.all()[:5]

class ProductDetailView(generic.DetailView):
    model = Product
    template_name = 'products/product_detail.html'

@login_required
@require_POST
def buy_product(request, product_id):
    customer = Customer.objects.get(user=request.user)
    product = Product.objects.get(pk=product_id)
    product_quantity = int(request.POST['quantity'])
    user_cart, created = Cart.objects.get_or_create(user = customer)
    entry_cart, created = CartEntry.objects.get_or_create(cart=user_cart, product=product)
    entry_cart.quantity += product_quantity
    entry_cart.buy(product_quantity)
    entry_cart.save()
    return HttpResponse(f"{product.title} (x{product_quantity}) added to your Cart.")

