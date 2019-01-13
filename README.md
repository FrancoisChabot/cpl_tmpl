# CPL_TMPL

A Jinja2-like template engine.

## Usage

**N.B.** All features displayed in this example do not 100% work yet.

```
#include "cpl_tmpl.h"
#include "json.hpp"

using json = nlohmann::json;

int main() {

  const char * raw_template = 
    "Hello, my name is {{name}}, and my friends are:"
    "{% for friend in friends %}"
    "  - {{friend}}"
    "{% endfor %}"

  // make a template
  auto template = cpl_tmpl::Interp_template(raw_template);

  // render the template:
  json data = {
    {"name" : "joe"},
    {"friends" : {"jack", "mike", "sue"}}
  };

  template.render(data, std::cout);

  return 0;
};
```