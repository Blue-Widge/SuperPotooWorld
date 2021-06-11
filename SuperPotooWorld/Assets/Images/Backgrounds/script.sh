for f in ./*.png; do    # remember to quote it or spaces may misbehave
    pngcrush -ow -rem allb -reduce $f
done
