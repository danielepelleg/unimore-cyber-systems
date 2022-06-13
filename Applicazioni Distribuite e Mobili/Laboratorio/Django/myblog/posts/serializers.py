from rest_framework.serializers import ModelSerializer
from accounts.serializers import UserSerializer
from .models import Author, Article

class AuthorDetailSerializer(ModelSerializer):
    user = UserSerializer()
    class Meta:
        model = Author
        fields = ['id','user', 'full_name', 'title', 'nationality']

class AuthorSerializer(ModelSerializer):
    class Meta:
        model = Author
        fields = ['id', 'full_name', 'title', 'nationality']

class ArticleSerializer(ModelSerializer):
    author = AuthorSerializer()
    class Meta:
        model = Article
        fields = ['id','title', 'content', 'pubblication_date', 'author']
