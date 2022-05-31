from django.http import HttpResponse
from django.shortcuts import get_object_or_404, render
from django.views import generic
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.decorators import login_required
from django.views.decorators.http import require_POST
from .forms import CustomerForm
from .models import Cart, CartEntry, Customer, Order, OrderEntry
import datetime

# Create your views here.

@login_required
def index(request):
    return render(request, 'users/index.html')

@login_required
def cart_detail(request, user_id):
    if user_id != request.user.id:
        return HttpResponse("You don't have the rights to see this.")
    current_customer = Customer.objects.get(user=request.user)
    user_cart = get_object_or_404(Cart, user=current_customer)
    products_list = {}
    for prod in user_cart.products.all():
        entry = CartEntry.objects.get(cart=user_cart, product=prod)
        products_list[prod.title] = entry.quantity
    return render(request, 'users/cart.html', {'cart': user_cart, 'products': products_list})

@login_required
@require_POST
def checkout(request, user_id):
    if user_id != request.user.id:
        return HttpResponse("You don't have the rights to do this.")
    customer = Customer.objects.get(user=request.user)
    user_cart = get_object_or_404(Cart, user=customer)
    order = Order.objects.create(user = customer, total=user_cart.total_cost)
    for prod in user_cart.products.all():
        product_fk = CartEntry.objects.get(cart=user_cart, product=prod)
        entry_order = OrderEntry.objects.create(
            order=order, 
            product=prod, 
            quantity = product_fk.quantity
        )
        entry_order.save()
    order.save()
    user_cart.delete()
    date_time = datetime.datetime.strftime(order.created_at, "%d-%M-%Y %H:%m:%S")
    return HttpResponse(f"Order {date_time} status: {order.get_status_display()}")

class OrderView(generic.ListView):
    template_name = 'users/orders.html'
    context_object_name = 'user_orders'

    def get_queryset(self) -> list:
        return Order.objects.filter(user = self.kwargs['user_id']).order_by('-created_at')[:5]

@login_required
def order_detail(request, user_id, order_id):
    if user_id != request.user.id:
        return HttpResponse("You don't have the rights to see this.")
    current_customer = Customer.objects.get(user=request.user)
    user_order = get_object_or_404(Order, pk=order_id, user=current_customer)
    products_list = {}
    for prod in user_order.products.all():
        entry = OrderEntry.objects.get(order=user_order, product=prod)
        products_list[prod.title] = entry.quantity
    return render(request, 'users/order_detail.html', {'order': user_order, 'products': products_list})

def register(request):
    if request.method == 'POST':
        user_form = UserCreationForm(request.POST)
        customer_form = CustomerForm(request.POST)
        if user_form.is_valid():
            user = user_form.save()
        if customer_form.is_valid():
            customer = customer_form.save(commit=False)
            customer.user = user    
            customer.save()
            return HttpResponse("Registration Success!")
    else:
        user_form = UserCreationForm()
        customer_form = CustomerForm()
        return render(request, 'registration/register.html', {'user_form': user_form, 'customer_form': customer_form})