# Maintainer: Gabriel A. Matos <gabriellramone@gmail.com>

pkgname=mohavim
pkgver=7.2
pkgrel=1
pkgdesc="Fork modernizado do GNU nano com interface melhorada"
arch=('x86_64' 'i686' 'aarch64')
url="https://"
license=('GPL3')
depends=('glibc')
makedepends=('gcc' 'make')
source=("$pkgname-$pkgver.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$srcdir/$pkgname-$pkgver/MohavimEditor"
    make clean
    make
}

package() {
    cd "$srcdir/$pkgname-$pkgver/MohavimEditor"
    
    # Instalar o executável
    install -Dm755 mohavim "$pkgdir/usr/bin/mohavim"
    
    # Instalar documentação
    install -Dm644 ../README.md "$pkgdir/usr/share/doc/$pkgname/README.md"
    
    # Instalar arquivos de sintaxe
    install -dm755 "$pkgdir/usr/share/$pkgname/syntax"
    cp -r syntax/* "$pkgdir/usr/share/$pkgname/syntax/"
}