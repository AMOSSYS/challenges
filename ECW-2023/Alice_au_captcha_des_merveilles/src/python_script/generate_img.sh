id=0
for i in \
    Pour Alice, il est crucial de maintenir le cap qu\'elle a choisi,\
    de persévérer avec une détermination inébranlable et de ne pas laisser\
    les épreuves décourager sa route. Les chemins vers le succès sont rarement\
    sans obstacles, mais ce sont justement ces obstacles qui offrent\
    à Alice l\'occasion de grandir et de se rapprocher de ses aspirations.\
    En continuant à avancer avec confiance et en tirant des enseignements\
    de chaque expérience, Alice peut façonner un avenir prometteur qui\
    lui est propre. Abandonner cette voie serait renoncer aux progrès\
    qu\'Alice a déjà accomplis. Bravo, vous y êtes presque encore 3 images.
do echo $i
    convert -quality 1 -background white -fill black -pointsize 80 label:$i ../server/images/img_$id.jpeg
    id=$((id+1))
done
