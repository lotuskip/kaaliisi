# Maintainer: Onni R. <onnir at iki dot fi>
pkgname=kaaliisi
pkgver=3
pkgrel=1
pkgdesc="A very minimalistic roguelike game."
url="http://github.com/lotuskip/kaaliisi"
arch=('i686' 'x86_64')
depends=('ncurses')
source=(http://github.com/downloads/lotuskip/$pkgname/$pkgname-$pkgver.tar.gz)
md5sums=('e36f2461a2a2f8df982cacfb75a8108c')

build() {
  cd $srcdir/source
  make || return 1

  install -m 755 -D $srcdir/$pkgname $pkgdir/usr/bin/$pkgname
}
