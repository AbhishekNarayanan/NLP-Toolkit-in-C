import time
# load data
filename = 'file.txt'
file = open(filename, 'rt')
text = file.read()
file.close()


# split into words
from nltk.tokenize import word_tokenize
start_tokenize=time.time()
tokens = word_tokenize(text)
end_tokenize=time.time()
#print(tokens[:20])
#print()


#clean data
# convert to lower case
import string
start_clean=time.time()
tokens = [w.lower() for w in tokens]
# remove punctuation from each word
table = str.maketrans('', '', string.punctuation)
stripped = [w.translate(table) for w in tokens]
# remove remaining tokens that are not alphabetic
words = [word for word in stripped if word.isalnum()]
end_clean=time.time()
#print(words[:20])
#print()

# filter out stop words
from nltk.corpus import stopwords
start_stopwords=time.time()
stop_words = set(stopwords.words('english'))
words = [w for w in words if not w in stop_words]
end_stopwords=time.time()
#print(words[:20])

# stemming of words
from nltk.stem.porter import PorterStemmer
start_stemming=time.time()
porter = PorterStemmer()
stemmed = [porter.stem(word) for word in words]
end_stemming=time.time()
#print(stemmed[:20])
#print()


#vectorizer
from sklearn.feature_extraction.text import CountVectorizer
f=open(filename,"r")
corpus=f.readlines();
corpus=[s[:len(s)-1] for s in corpus]
start_vectorize=time.time()
vectorizer = CountVectorizer(stop_words='english',binary=False, max_features=5)
X = vectorizer.fit_transform(corpus)
end_vectorize=time.time()
print(X.toarray().astype(int))
print(vectorizer.get_feature_names())


#similarity
from collections import Counter
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.metrics.pairwise import cosine_similarity
def get_cosine_sim(*strs): 
    vectors = [t for t in get_vectors(*strs)]
    return cosine_similarity(vectors)
    
def get_vectors(*strs):
    text = [t for t in strs]
    vectorizer = CountVectorizer(text)
    vectorizer.fit(text)
    return vectorizer.transform(text).toarray()
start_similarity=time.time()
print(get_cosine_sim("Hey how are you","Hey how is he"))
end_similarity=time.time()

#print times
print("Time for Tokenization is "+str(end_tokenize-start_tokenize))
print("Time for Cleaning is "+str(end_clean-start_clean))
print("Time for Stop Word Removal is "+str(end_stopwords-start_stopwords))
print("Time for Stemming is "+str(end_stemming-start_stemming))
print("Time for Vectorization is "+str(end_vectorize-start_vectorize))
print("Time for Cosine Similarity is "+str(end_similarity-start_similarity))
