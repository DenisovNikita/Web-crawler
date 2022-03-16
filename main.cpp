#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cassert>

class Crawler {
private:
    const int TOP = 5;

    // get top five
    std::set<std::pair<int, std::string_view>, std::greater<>> top_ranks;
    // calc ranks
    std::map<std::string_view, int> page_ranks;
    // queue for yet unvisited urls
    std::queue<std::string_view> holder;
    
    void add_new_url(const std::string_view &url) {
        holder.push(url);
        assert(page_ranks.count(url) == 0);
        page_ranks[url] = 0;
    }

    void increase_rank(const std::string_view &url) {
        if (page_ranks.count(url)) {
            top_ranks.erase(std::pair{page_ranks[url], url});
        }
        page_ranks[url]++;
        top_ranks.insert(std::pair{page_ranks[url], url});
    }

    bool is_url_new(const std::string_view &url) {
        return page_ranks.count(url) == 0;
    }

    auto get_html(const std::string_view &url) {
        // TODO: use libcurl
    }

    auto get_hyperlinks(auto html) {
        // TODO: parse html, get <a> tags
        std::vector<std::string_view> hyperlinks;
        for (const auto &tag : html.find_all("a")) {
            hyperlinks.push_back(tag.content);
        }
        return hyperlinks;
    }

    // add the content of newly found page url
    void add_hyperlinks(const std::string_view &root_url) {
        auto html = get_html(root_url);
        std::vector<std::string_view> hyperlinks = get_hyperlinks(html);
        for (const auto &url : hyperlinks) {
            if (is_url_new(url)) {
                add_new_url(url);
            }
            increase_rank(url);
        }
    }

    void visit_new_urls() {
        while (!holder.empty()) {
            auto url = holder.front();
            holder.pop();
            add_hyperlinks(url);
        }
    }

public:
    // add newly found page url
    // O(recursive hyperlinks)
    void add_url(const std::string_view &root_url) {
        if (is_url_new(root_url)) {
            add_new_url(root_url);
            visit_new_urls();
        }
    }

    // return top 5 most important pages
    // O(1)
    [[nodiscard]] std::vector<std::string_view> query_top_five(
            const std::string_view &root_url) const {
        std::vector<std::string_view> result;
        auto it = top_ranks.begin();
        for (int i = 0; i < std::min(TOP, static_cast<int>(top_ranks.size())); i++) {
            result.push_back((it++)->second);
        }
        return result;
    }

    // return a list of pages which urls have same prefix
    // O(log(urls))
    [[nodiscard]] std::vector<std::string_view> query_list_of_pages_with_given_prefix(
            const std::string_view &prefix) const {
        auto lower_it = page_ranks.lower_bound(prefix);
        auto upper_it = page_ranks.upper_bound(prefix);
        std::vector<std::string_view> result;
        for (auto it = lower_it; it != upper_it; it++) {
            result.push_back(it->first);
        }
        return result;
    }
};


int main() {
    std::string_view example_url("https://www.chess.com/");
    Crawler crawler;
    crawler.add_url(example_url);
    std::vector<std::string_view> top = crawler.query_top_five(example_url);
    for (const auto &x : top) {
        std::cout << x << "\n";
    }
    std::vector<std::string_view> result = crawler.query_list_of_pages_with_given_prefix("https://");
    for (const auto &x : result) {
        std::cout << x << "\n";
    }
}
