""" convert the all_English_word.xlm to the all_English_word.json """
import json
import xml.dom.minidom
from lxml import etree
import xml.etree.ElementTree as ElementTree

# parse the translation

file_path = "./all_English_word.xml"
save_file_path = "./all_English_word.json"
dom = xml.dom.minidom.parse(file_path)
xml_words = dom.getElementsByTagName("word")
xml_tags = dom.getElementsByTagName("tags")
xml_trans = dom.getElementsByTagName("trans")
#print(xml_trans)

words_list = [xml_word.firstChild.data for xml_word in xml_words]
tags_list = [xml_tag.firstChild.data for xml_tag in xml_tags]
trans_list = list()
with open(file_path, "r+", encoding="UTF-8") as f:
    str_content = f.read()
    root = etree.fromstring(str_content)
    for translate in root.xpath("//trans"):
        trans_list.append(translate.text)
# create the set
tags_set = set(tags_list)
# creat the dict
all_words_dict = dict()
for tag in tags_set:
    all_words_dict[tag] = dict()
#zip_words = zip(words_list, tags_list, trans_list)
zip_words = zip(words_list, tags_list, trans_list)
for word, tag, tran in zip_words:
    word_dict = dict()
    word_dict[word] = {"label": "-", "translate": tran}
    all_words_dict[tag].update(word_dict)

# save the all_words_dict to the json file
with open(save_file_path, "w+", encoding="UTF-8") as f:
    #json.dump(all_words_dict, f, ensure_ascii=False, indent=4)
    is_save = False
    if is_save:
        json.dump(all_words_dict, f, ensure_ascii=False)
