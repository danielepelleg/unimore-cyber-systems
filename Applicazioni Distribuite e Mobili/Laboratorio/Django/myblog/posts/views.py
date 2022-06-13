from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from django.views import generic
from rest_framework.parsers import JSONParser
from accounts.models import User
from .models import Article, Author
from .forms import AuthorForm, ArticleForm
from .serializers import ArticleSerializer
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_POST


# Create your views here.

def index(request):
    return render(request, 'posts/index.html')

class PostsYearMonthListView(generic.ListView):
    template_name = "posts/list_detail.html"
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        user = User.objects.get(id=self.kwargs['user_id'])
        author = Author.objects.get(user=user)
        return Article.objects.filter(author = author) \
                .filter(pubblication_date__year = self.kwargs['year']) \
                .filter(pubblication_date__month = self.kwargs['month']) \
                .order_by('-pubblication_date')[:5]
    
    def get_context_data(self):
        context = super().get_context_data(**self.kwargs)
        return context

class UserPostsListView(generic.ListView):
    template_name = "posts/list.html"
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        user = User.objects.get(id=self.kwargs['user_id'])
        author = Author.objects.get(user=user)
        return Article.objects.filter(author = author) \
                .order_by('-pubblication_date')[:5]
    
    def get_context_data(self, **kwargs):
        context = super().get_context_data(**self.kwargs)
        return context
        
class PostDetailView(generic.DetailView):
    model = Article
    context_object_name = 'element'
    template_name = "posts/detail.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**self.kwargs)
        return context

def insert_author(request):
    if request.method == 'POST':
        form = AuthorForm(request.POST)
        form.save()
        return HttpResponse('New Author has been saved')
    else:
        form = AuthorForm()    
        return render(request, 'posts/form.html', {'form': form})

@login_required
def edit_post(request, element_id):
    if request.method == 'POST':
        article = Article.objects.get(pk=element_id)
        form = ArticleForm(request.POST, instance=article)
        if form.is_valid():
            form.save()
            return render(request, 'posts/redirect.html', {'message': 'Article Edit Success!'})
        return render(request, 'posts/redirect.html', {'message': 'Article Edit Error.'})
    else:
        article = Article.objects.get(pk=element_id)
        form = ArticleForm(instance=article)    
        return render(request, 'posts/form.html', {'form': form})

@require_POST
def search(request):
    user_id = request.POST.get('user_id')
    print(user_id)
    user = User.objects.get(pk=user_id)
    author = Author.objects.get(user=user)
    articles = Article.objects.filter(author=author)
    return render(request, 'posts/anonymous_list.html', {'elements_list': articles})


@login_required
def insert_article(request):
    if request.method == 'POST':
        form = ArticleForm(request.POST)
        author = Author.objects.get(user=request.user)
        if form.is_valid():
            article = form.save(commit=False)
            article.author = author
            article.save()
            return render(request, 'posts/redirect.html', {'message': 'Article Insertion Success!'})
        return render(request, 'posts/redirect.html', {'message': 'Article Insertion Error.'})
    else:
        form = ArticleForm()    
        return render(request, 'posts/form.html', {'form': form})

def jquery_list(request, user_id, year, month):
    url = f"/myblog/api/{user_id}/posts/{year}/{month}"
    return render(request, 'posts/jquery.html', {'url': url, 'script_name': 'list.js'})

def jquery_detail(request, user_id, year, month, post_id):
    url = f"/myblog/api/{user_id}/posts/{year}/{month}/{post_id}"
    return render(request, 'posts/jquery.html', {'url': url, 'script_name': 'detail.js'})

"""
    API
"""
@csrf_exempt
def detail_post_api(request, post_id):
    if request.method == 'GET':
        article = Article.objects.get(pk = post_id)
        serializer = ArticleSerializer(article)
        return JsonResponse(serializer.data, safe=False)
    elif request.method == 'DELETE':
        article = Article.objects.get(pk=post_id)
        print(article)
        article.delete()
        return JsonResponse(None, status=204, safe=False)
    elif request.method == 'POST':
        data = JSONParser().parse(request)
        article = Article.objects.get(pk = post_id)
        article.title = data['title']
        article.content = data['content']
        article.save()
        serializer = ArticleSerializer(article)
        return JsonResponse(serializer.data, status=201, safe=False)

"""
    API
"""
@csrf_exempt
def list_year_month_posts_api(request, user_id, year, month):
    if request.method == 'GET':
        user = User.objects.get(id=user_id)
        author = Author.objects.get(user=user)
        articles = Article.objects.filter(author = author) \
                .filter(pubblication_date__year = year) \
                .filter(pubblication_date__month = month) \
                .order_by('-pubblication_date')
        serializer = ArticleSerializer(articles, many=True)
        return JsonResponse(serializer.data, safe=False)

@csrf_exempt
def detail_year_month_posts_api(request, user_id, year, month, post_id):
    if request.method == 'GET':
        user = User.objects.get(id=user_id)
        author = Author.objects.get(user=user)
        article = Article.objects.get(pk=post_id, author = author, pubblication_date__year = year, pubblication_date__month = month)
        serializer = ArticleSerializer(article)
        return JsonResponse(serializer.data, safe=False)
