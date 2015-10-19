#include <stdio.h>

#include <fstream>
#include <unordered_map>
#include <vector>

namespace {
#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char *);
}

int main(int argc, char **argv) {
#ifdef TEST
  test();
#else  //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
  return 0;
}

namespace {

void process(std::string s);

#ifdef TEST
void test() {
  std::vector<std::string> v_test{
      "|deEva|lan t|to ha|evil |ankin|il-ev|o hac| to h|vil "
      "p|an to|The e|CodeE| evil|plan |hack |Eval |ack C|l "
      "ran|king.|l-evi|evil-|-evil|l pla|il pl| hack|al "
      "ra|vil-e|odeEv|he ev|n to |ck Co|eEval|nking| rank| "
      "Code|e evi|ranki|k Cod| plan|val r|"};
  std::vector<std::string> v_expect{
      "The evil-evil plan to hack CodeEval ranking."};
  for (int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    printf("%s\n", &v_expect[i][0]);
  }
}
#endif //#ifdef TEST

void process_file(char *path) {
  std::ifstream stream(path);
  for (std::string line; std::getline(stream, line);) {
    process(line);
  }
}

struct Item {
  std::string value;
  int left;
  int right;
  Item *prev = nullptr;
  Item *next = nullptr;

  Item(char *begin, char *end) : value(begin, end) {}
};

struct Part {
  int leftCount = 0;
  Item *left = nullptr;

  int rightCount = 0;
  Item *right = nullptr;
};

void process(std::string line) {
  std::vector<Item> items;
  for (size_t i = 1, j = line.find('|', 1); j != std::string::npos;
       i = j + 1, j = line.find('|', i)) {
    items.emplace_back(&line[i], &line[j]);
  }

  const int pieceSize = items.front().value.length() - 1;

  std::unordered_map<int, Part> parts;
  {
    std::unordered_map<std::string, int> ids;
    for (size_t i = 0; i < items.size(); ++i) {
      auto &item = items[i];
      if (item.value.length() - 1 != pieceSize)
        throw item;

      auto &left = ids[std::string(item.value.begin(), item.value.end() - 1)];
      if (!left)
        left = ids.size();
      item.left = left;

      parts[left].leftCount++;
      parts[left].left = &item;

      auto &right = ids[std::string(item.value.begin() + 1, item.value.end())];
      if (!right)
        right = ids.size();
      item.right = right;

      parts[right].rightCount++;
      parts[right].right = &item;
    }
  }

  // pass 1 - just glue unambigous pieces
  for (size_t i = 0; i < items.size(); ++i) {
    auto &item = items[i];
    auto &r = parts[item.right];
    if (r.rightCount == 1 && r.leftCount == 1) {
      item.next = r.left;
      item.next->prev = &item;
      r.leftCount = r.rightCount = 0;
    }
  }

  // pass 2 - build glued parts
  std::vector<Item> glued;
  for (auto &item : items) {
    if (nullptr != item.prev) {
      continue;
    }
    glued.emplace_back(&item.value[0], &item.value[item.value.length()]);
    auto &g = glued.back();
    g.left = item.left;
    for (auto ptr = item.next; ptr; ptr = ptr->next) {
      g.value.push_back(ptr->value.back());
      g.right = ptr->right;
    }
  }

  for (auto &item : glued) {
    if (nullptr != item.next) {
      continue;
    }
    Item *next = nullptr;
    int rl = 0;
    for (auto &ref : glued) {
      if (ref.left != item.right) {
        continue;
      }
      if (&ref == &item || nullptr != ref.prev) {
        continue;
      }
      if (parts[ref.right].leftCount >= rl) {
        rl = parts[ref.right].leftCount;
        next = &ref;
      }
    }
    if (nullptr == next) {
      continue;
    }
    item.next = next;
    next->prev = &item;

    auto &p = parts[item.right];
    p.leftCount--;
    p.rightCount--;
  }

  Item *head = nullptr;
  for (auto &i : glued) {
    if (nullptr == i.prev) {
      head = &i;
      break;
    }
  }

  printf("%s", &head->value[0]);
  while ((head = head->next)) {
    printf("%s", &head->value[pieceSize]);
  }
  printf("\n");
}
}
