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
## n-grams
n relative to the memory length of model
### Markov Assumption
$$P(w_i|w_1w_2...w_{i-1}) \approx P(w_{i-n+1}...w_{i-1}) $$

### Maximun Likelihood Estimation
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
* Kneser-Ney Smoothing: redistribute probability mass based on the versatility (continuation probability) of the lower order n-gram