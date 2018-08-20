def append_to_string(sentence : str):
    if len(sentence) >= 5:
        return f"{sentence} World"
    else:
        return f"Welcome, {sentence}"


assert append_to_string("Hello") == "Hello World", "Fail"
assert append_to_string("Hi") == "Welcome, Hi", "Fail"

#print(append_to_string("Hello"))


#list = ["AC","ABAA","BB"]
#print(min(list))

########################################################################################################################

def filter_list(list):
    for i in list:
        if type(i) is int:
            yield i


def filter_list2(list):
    return [i for i in list if type(i) is int]


#my_list = list(filter_list([1, 5, 'A', 30, 'Hello', 50, 2.75]))

#print(my_list)
########################################################################################################################

def who_likes_it(listOfLikes):
    likesCount = len(listOfLikes)

    response = {
        0: "no one likes this",
        1: "{} likes this",
        2: "{} and {} like this",
        3: "{}, {} and {} like this",
        4: "{}, {} and " + str(likesCount-2) + " others like this"
    }

    return response.get(min(likesCount,4)).format(*listOfLikes)


assert who_likes_it([]) == "no one likes this", "Wrong like count!"
assert who_likes_it(["Ryszard"]) == "Ryszard likes this", "Wrong like count!"
assert who_likes_it(["Marcin", "Michal"]) == "Marcin and Michal like this", "Wrong like count!"
assert who_likes_it(["Edyta", "Igor", "Kamil"]) == "Edyta, Igor and Kamil like this", "Wrong like count!"
assert who_likes_it(["Michal", "Maciej", "Bartosz", "Przemek"]) == "Michal, Maciej and 2 others like this", "Wrong like count!"
########################################################################################################################


def count_duplicates(sentence, how_many_times):
    for i in range(255):
        if sentence.count(chr(i)) != how_many_times:
            sentence = sentence.replace(chr(i), '')

    sentence_no_duplicates = ''
    for c in sentence:
        if sentence_no_duplicates.count(c) == 0:
            sentence_no_duplicates += c

    return sentence_no_duplicates


assert count_duplicates("aabcdddee", 2) == "ae", "Failed to count!" # only 'a' end 'e' were present 2 times.
assert count_duplicates("indivisibility", 6) == "i", "Failed to count!"
assert count_duplicates("Karima", 1) == "Krim", "Failed to count!"
########################################################################################################################

def reorder_substrings(sentence):

    def comparator(word):
        for c in word:
            if c.isdigit():
                return c, word

    sentence = sentence.split(' ')
    sentence = sorted(sentence, key=comparator)
    return ' '.join(sentence)


print(reorder_substrings("is2 Thi1s T4est 3a"))
assert reorder_substrings("is2 Thi1s T4est 3a") == "Thi1s is2 3a T4est", "Failed!"

