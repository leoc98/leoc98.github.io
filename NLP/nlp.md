# text preprocessing
* words => sentence => documents => corpus(like wikipedia)
* word token: each instance of a word
* word type: distinct words
## reason
language is compositional, to break long sentence into individual words for machine to process
## steps
1. remove unwanted formatting (e.g. HTML)
2. sentence segmentation: break documents into sentences
3. word wokenisation: break sentences into words
4. word normalisation: words => canonical forms (e.g. Hi => hi)
5. stopword removal: delete unwanted words

## Sentence segmentation
use binary classifier to determine each segemntation flag (e.g. punchation)

* Features
    * Look at the words before and after "." 
    * Word shapes:
        * -Uppercase, lowercase,ALL CAPS, numbel
        * -Character length
* Part-of-speech tags:
    * Determiners tend to start a sentence

## Word Tokenisation
Assumption: good vocabulary and lexicon

MaxMatch algorithm

a greedy match longest word in vocabulary

failed: 去买/新西兰/花;去买新/西兰花

### Subword Tokenisation
bring more meaningful word breakdown (e.g. colourless => colour less)

#### Byte-pair encoding (BPE)
to build a vocabulary by iterly find the words combination from most frequently to most rearly. *Use a threshold to seperate a valid vocabulary set to use.*

Advantage:
* Data-informed tokenisation
* Works for different languages (tokens)
* Deals better with unknown words

### Word normalisation
reduce the size of vocabulary

#### Morphology
* inflectional: not changing meaning
    * Lemmatisation: remove all inflection
* derivational: 
    * suffix: change lexical category
    * prefix: change meaning but not change lexical category
    * Stemming: strips off all prefixes and suffixes
        * good for information retrieval(searching)


## Stopword removal
def: stop word, a list of word that will be remove from vocabulary

# Language Models
## Seen probability
$$P(sentence) = \prod_{i<len(sentence)} P(w_i)$$
## Perplexity
$$
PP(sentence) = (\frac{1}{P(sentence)})^{ \frac{1}{m}}
$$
$$
log(PP) = -\frac{1}{m}log(P)
$$
## n-grams
### Markov Assumption
$$
P(w_i|w_1w_2...w_{i-1}) \approx P(w_{i-n+1}...w_{i-1}) 
$$

### Maximun Likelihood Estimation
unigram
$$
P(w_i) = \frac{C(w_i)}{M}
$$
n-gram($$n>1$$)
$$P(w_{i-n+1}...w_{i-1}) = \frac{
C(w_{i-n+1}...w_{i})
}{
C(w_{i-n+1}...w_{i-1})
}$$

### Smoothing
avoid meet some events that have never met before

* Laplacian Smoothing $P(w_i) = \frac{
C(w_{i-n+1}...w_{i}) + \alpha
}{
C(w_{i-n+1}...w_{i-1}) + \alpha|V|
}$
* Lidstone Smoothing $P(w_i) = \frac{
count(word) + \alpha
}{
word\_token + word\_type \times \alpha
}$
* Absolute Discounting: take away fix probability from existed words to unseen words 
* Katz Backoff: take away fix probability from existed words and redistribute to unseen words
    1. get a fix count from existed words
    2. distribute fix count by the portion of all unseen combination (where $$C(w_{i-1},w_a) = 0$$)
        * $$\frac{P(this)}{\sum_{C(w_{i-1},w_a) = 0} P(w_{a})}$$
        * note that $$P(w_i)$$ is a backoff, which means it takes the probability of (n-1)-grams
* Kneser-Ney Smoothing: redistribute probability mass based on the versatility (continuation probability) of the lower order n-gram
    1. get a fix count from existed words
    2. distribute fix count by the portion of all existed conbination prefix count (**unique count**, where $$count(w^{'}_{i-1}:C(w^{'}_{i-1}, w_i)>0)$) divided all unseen combindation of their seen count (**also unique count**, where $\sum_{\underbrace{w_j:C(w_{i-1},w_{j})=0}_{all\ unseen\ combindation\ of\ w_{i-1}}} count(\underbrace{w_j:C(w_{j-1},w_{j}) > 0}_{seen\ conbination\ count\ of\ w_j})$$)
### Interpolation
$$
P_{IN}(w_i|w_{i-1},w_{i-2}) = \lambda_3 P_3 + \lambda_2 P_2 + \lambda_1 P_1
$$
where $$\sum \lambda_i = 1$$, 
$$P_i$$ denotes i-grams probability,
$$lambda$$ is learned base on held out data


# Text Classification
## Category
### Topic Classification
* Motivation:library science, information retrieval
* Classes: Topic categories, e.g.“jobs”, “internationalnews’
* Features
    * Unigram bag of words (BOW), with stop-words removed
    * Longer n-grams (bigrams, trigrams) for phrases
* Examples of corpora
    * Reuters news corpus (RCV1; NLTK)
    * Pubmed abstracts
    * Tweets with hashtags
### Sentiment Analysis

* Motivation: opinion mining, business analytics
* Classes: Positive/Negative/(Neutral)
* Features
    * N-grams
    * Polarity lexicons
* Examples of corpora
    * Movie review dataset (in NLTK)
    * SEMEVAL Twitter polarity datasets

### Native-Language Identification
* Motivation: forensic linguistics, educational applications
* Classes: first language of author (e.g. Indonesian)
* Features
    * Word Ngrams
    * Syntactic patterns (POS, parse trees)
    * Phonological features

### Natural Language Inference
find relationship between two sentence.
* Motivation: language understanding
* Classes:entailment, contradiction, neutral
* Features
    * Word overlap
    * Length difference between the sentences
    * N-grams

## Build text classifier
* identify a task
* collect appropriate corpus
* carry out annotation
* select features
* choose algorithm
* train model & tune hyper parameter

### algorithm

use SVM
* non-linear kernel trick works well for text
* NLP has a good feature scaling
* NLP dataset is large
* NLP problems often involve large feature sets

# Part of Speech (POS)
for information extraction

## class
* open: new words will be generated
    * nouns
    * verbs
    * adjectives
    * adverbs
* closed: no new words
    * prepositions(in, on, with, ...)
    * paricles:
        * breushed himself **off**
    * determiners(a, an, the,... )
    * pronouns(I, me,my,...)
    * conjunctions(and, or, if,...)
    * modal verbs(can,could,should,must,...)

## Tagsets
...

### automiatic Tagger
* rule-based tagging
    * find a list of possible tags for each word
    * apply rules to narrow down to a single tag 
* unigram Tagger
    * assign most common tag to each word type
    * model is a look-up table
* n-gram Tagger
    * aparse issues (use backoff)
* classifier-based tagging
    * use a classifier to tag each features
    * error propagation: wrong prediction infect next prediction
* hidden markov Models
    * use a basic sequential model
    * takes into account the whole sequence (avoid error propagation) (call sequence labelling, structured prediction)
    * output independence assumption: ignored previous tags influence to current word
        * $$P(w|t) = \prod P(w_i|t_i)$$
    * markov assumption: tag can only relavant to previous tag
        * $$P(t) = \prod P(t_i|t_{i-1})$$
    * unable to track all possibility
        * use viterbi algorithm
        * a dynamic programming way
        * $$P(w|t)P(t) = \prod P(w_i|t_i)*max(P(t_i|t_{i-1})*s(t_{i-1}, w_{i-1}))$$
        * where $$s(t_{i-1}, w_{i-1})$$ is the previous state that has been calculate (store in a table)
        * $$O(t^2n)$$, where t is the tagset size, n is the lenght of sequence

### unkonwn words
* use hapax legomena to guess
* sub-word representations to capture morphology

# Deep leanrning for NLP

