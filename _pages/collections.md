---
title: "Collections"
# layout: categories
permalink: /collections/
author_profile: true
---

{% for collection in site.collections %}
{% unless collection.label == "posts" %}
<h2> {{ collection.name }} </h2>
<ul>
{% for post in site[collection.label] %}
<li><a href="{{ post.url }}">{{ post.title }}</a></li>
{% endfor %}
</ul>
{% endunless %}
{% endfor %}
