from argparse import ArgumentParser as ap
import numpy as np

def make_v0_map(args):
  lines = []
   
  offline_number = 0

  link_chans = {
    0:np.arange(0, 4),
    1:np.arange(4, 8),
    2:np.arange(10, 14),
    3:np.arange(14, 18),
  }

  for slot in [4, 5]:
    for link, chans in link_chans.items():
      for c in chans: 
        lines.append(f'{slot} {link} {c} {offline_number}\n')
        offline_number += 1

  slot = 7
  for link in range(2):
    for c in link_chans[link]: 
      lines.append(f'{slot} {link} {c} {offline_number}\n')
      offline_number += 1

  link = 0
  for slot in [9, 11]:
    for c in range(40):
      lines.append(f'{slot} {link} {c} {offline_number}\n')
      offline_number += 1

  slot = 13
  for c in range(0, 16):
    lines.append(f'{slot} {link} {c} {offline_number}\n')
    offline_number += 1

  slot = 12
  for c in range(0, 4):
    lines.append(f'{slot} {link} {c} {offline_number}\n')
    offline_number += 1

  slot = 13
  for c in range(20, 40):
    lines.append(f'{slot} {link} {c} {offline_number}\n')
    offline_number += 1

  with open(args.o, 'a') as f:
    f.writelines(lines)

if __name__ == '__main__':
  parser = ap() 
  parser.add_argument('-o', required=True)
  parser.add_argument('-v', default=0, type=int)
  args = parser.parse_args()

  makers = {
   0:make_v0_map, 
  }

  if args.v not in makers.keys():
    exit() #TODO -- print something

  makers[args.v](args)
