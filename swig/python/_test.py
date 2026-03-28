from Postbinary import Pb64_32p
from Postbinary import Pb128_32ip
from Postbinary import Package
from Postbinary import Compressor

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    min_val = 17.035
    max_val = 17.878
    pb64 = Pb64_32p()
    pb64.convertFromFloat(min_val, max_val)
    print(f'Number:      {pb64.toString()}, bytes = {pb64.sizeInBytes()}')

    package = Compressor.compress(pb64, 12)
    print(f'Compres:   {package.toString()}, bytes = {package.sizeInBytes()}')

    newPb64 = Compressor.decompressAsPb64(package)
    print(f'Decompres:   {newPb64.toString()}, bytes = {newPb64.sizeInBytes()}')
    l, r = newPb64.getMinimumFloatRange()
    print(f'Range: [{l}, {r}]')



    print(" ")

    min_val = -17.035
    max_val = 12332
    pb128 = Pb128_32ip(min_val, min_val, max_val, max_val)
    print(f'Number:        {pb128.toString()}, bytes = {pb128.sizeInBytes()}')

    package = Compressor.compress(pb128, 12)
    print(f'Compres:   {package.toString()}, bytes = {package.sizeInBytes()}')

    newPb128 = Compressor.decompressAsPb128(package)
    print(f'Decompres:     {newPb128.toString()}, bytes = {newPb128.sizeInBytes()}')
    l = newPb128.getLeft()
    l1, l2 = l.getMinimumFloatRange()
    r = newPb128.getRight()
    r1, r2 = r.getMinimumFloatRange()
    print(f'Range: [({l1}, {l2}), ({r1}, {r2})]')





    # base64.b64encode(pb64.toBytes()).decode('utf-8')

    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
