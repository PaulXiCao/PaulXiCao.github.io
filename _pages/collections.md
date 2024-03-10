---
title: "Collections"
permalink: /collections/
read_time: false
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
