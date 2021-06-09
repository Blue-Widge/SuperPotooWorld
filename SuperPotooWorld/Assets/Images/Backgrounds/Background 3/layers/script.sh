for f in ./*.png; do
    # do some stuff here with "$f"
    # remember to quote it or spaces may misbehave
    pngcrush -ow -rem allb -reduce $f
done
