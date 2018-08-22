def append_to_string(sentence: str):
    if len(sentence) >= 5:
        return f"{sentence} World"
    else:
        return f"Welcome, {sentence}"


assert append_to_string("Hello") == "Hello World", "Fail"
assert append_to_string("Hi") == "Welcome, Hi", "Fail"


# print(append_to_string("Hello"))


# list = ["AC","ABAA","BB"]
# print(min(list))

########################################################################################################################

def filter_list(list):
    for i in list:
        if type(i) is int:
            yield i


def filter_list2(list):
    return [i for i in list if type(i) is int]


# my_list = list(filter_list([1, 5, 'A', 30, 'Hello', 50, 2.75]))

# print(my_list)
########################################################################################################################

def who_likes_it(listOfLikes):
    likesCount = len(listOfLikes)

    response = {
        0: "no one likes this",
        1: "{} likes this",
        2: "{} and {} like this",
        3: "{}, {} and {} like this",
        4: "{}, {} and " + str(likesCount - 2) + " others like this"
    }

    return response.get(min(likesCount, 4)).format(*listOfLikes)


assert who_likes_it([]) == "no one likes this", "Wrong like count!"
assert who_likes_it(["Ryszard"]) == "Ryszard likes this", "Wrong like count!"
assert who_likes_it(["Marcin", "Michal"]) == "Marcin and Michal like this", "Wrong like count!"
assert who_likes_it(["Edyta", "Igor", "Kamil"]) == "Edyta, Igor and Kamil like this", "Wrong like count!"
assert who_likes_it(
    ["Michal", "Maciej", "Bartosz", "Przemek"]) == "Michal, Maciej and 2 others like this", "Wrong like count!"


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


assert count_duplicates("aabcdddee", 2) == "ae", "Failed to count!"  # only 'a' end 'e' were present 2 times.
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


# print(reorder_substrings("is2 Thi1s T4est 3a"))
# assert reorder_substrings("is2 Thi1s T4est 3a") == "Thi1s is2 3a T4est", "Failed!"

########################################################################################################################
#                                                     DZIEN 2                                                          #
########################################################################################################################

# test_dict = {"test" : 0,"rest":3,"koks":9}
# print(test_dict)

def count_occurences(sentence):
    words = sentence.split(' ')
    words_occurences = {}
    for word in words:
        if word in words_occurences:
            words_occurences[word] += 1
        else:
            words_occurences[word] = 1
    return words_occurences


def count_occurences2(sentence):
    words = ''.join([c for c in sentence if (c.isalnum() or c.isspace())])
    words = words.split(' ')
    words_occurences = {}
    for word in words:
        if word in words_occurences:
            words_occurences[word] += 1
        else:
            words_occurences[word] = 1

    non_al_num_occurences = {}
    non_al_nums = [c for c in sentence if not (c.isalnum() or c.isspace())]
    for c in non_al_nums:
        if c in non_al_num_occurences:
            non_al_num_occurences[c] += 1
        else:
            non_al_num_occurences[c] = 1

    return {**words_occurences, **non_al_num_occurences}


# print(count_occurences2("Ala ma((((((( kota. Ala; ma psa."))

#######################################################################################################################
import re


def validatePIN(PIN):
    return bool(re.match(r"^\d{4}$", PIN))


assert validatePIN("1234") == True, "Wrong validation!"
assert validatePIN("12345") == False, "Wrong validation!"
assert validatePIN("a234") == False, "Wrong validation!"


########################################################################################################################


def validate_input(word):
    # Write a simple regex to validate a username. Allowed characters are:
    # lowercase letters, numbers, underscore
    # Length should be between 5 and 20 characters (both included).
    return bool(re.match(r"^[a-z]|\d|_{5,20}$", word))


assert validate_input("Summer Academmy") == False, "Bad validation!"
assert validate_input("Summer_Academmy") == False, "Bad validation!"
assert validate_input("summer_academmy") == True, "Bad validation!"


########################################################################################################################
#                                               dodatkowe                                                              #
########################################################################################################################


def valid_braces(sentence):
    '''
    Function takes a string of braces, and determines if the order of the braces is valid.
    It should return true if the string is valid, and false if it's invalid.
    A string of braces is considered valid if all braces are matched with the correct brace.
    '''
    return sentence


# assert valid_braces("(){}[]") == True, "Wrong answer!"
# assert valid_braces("([{}])") == True, "Wrong answer!"
# assert valid_braces("(}") == False, "Wrong answer!"
# assert valid_braces("[(])") == False, "Wrong answer!"

########################################################################################################################


def parenthesis_remover(words_list):
    result = []
    for word in words_list:
        cleanedWord = word
        for parenthesis in re.findall(r'[(].*?[)]', cleanedWord):
            cleanedWord = cleanedWord.replace(parenthesis, '')
        result.append(cleanedWord)
    return result


print(parenthesis_remover(["example (.com)", "w3resource", "github (.com)", "sta(siemanko)ckoverflow (.com)"]))
