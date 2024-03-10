# My personal page on GitHub

This blog is mostly about IT-related content, e.g. C++.
It is available under: https://paulxicao.github.io/.

# How to start your own page/blog hosted on GitHub

Click [**Use this template**](https://github.com/mmistakes/mm-github-pages-starter/generate) button above for the quickest method of getting started with the [Minimal Mistakes Jekyll theme](https://github.com/mmistakes/minimal-mistakes).

(I might create a separate blog post about it.)

# Build locally

```bash
sudo pacman -S docker
sudo systemctl start docker
sudo docker pull jekyll/jekyll
mkdir -p vendor/bundle
sudo docker run --rm --volume="$PWD:/srv/jekyll:Z" --volume="$PWD/vendor/bundle:/usr/local/bundle:Z" --publish [::1]:4000:4000 jekyll/jekyll jekyll serve
// open localhost::4000 in browser
```

Source: [github jekyll-docker](https://github.com/envygeeks/jekyll-docker/blob/master/README.md)
