from django.shortcuts import render
from django.http import HttpResponse
from django.views import generic
from .models import Article
from .forms import AuthorForm, ArticleForm

# Create your views here.

def index(request):
    return render(request, 'posts/index.html')

class PostsListView(generic.ListView):
    template_name = "posts/article_list.html"
    context_object_name = "latest_post_list"

    def get_queryset(self) -> list:
        return Article.objects.filter(author__id = self.kwargs['author']) \
                .filter(pubblication_date__year = self.kwargs['year']) \
                .filter(pubblication_date__month = self.kwargs['month']) \
                .order_by('-pubblication_date')[:5]
    
    def get_context_data(self):
        context = super().get_context_data(**self.kwargs)
        return context

class PostDetailView(generic.DetailView):
    model = Article
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
        return render(request, 'posts/insertForm.html', {'form': form})
    

def insert_article(request):
    if request.method == 'POST':
        form = ArticleForm(request.POST)
        form.save()
        return HttpResponse('New Article has been saved')
    else:
        form = ArticleForm()    
        return render(request, 'posts/insertForm.html', {'form': form})