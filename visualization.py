import matplotlib.pyplot as plt
import pandas as pd


def load_visualization_data(filename):
  original_cones = []
  merged_cones = []
  hull_points = []

  with open(filename, 'r', encoding='utf-8') as f:
    lines = f.readlines()

  section = None
  for line in lines:
    line = line.strip()
    if not line:
      continue

    if line == 'ORIGINAL_CONES':
      section = 'orig'
      continue
    elif line == 'MERGED_CONES':
      section = 'merged'
      continue
    elif line == 'CONVEX_HULL':
      section = 'hull'
      continue

    parts = line.split(',')
    if section == 'orig' and not line.startswith('id'):
      original_cones.append(
          {'id': parts[0], 'x': float(parts[1]), 'y': float(parts[2])}
      )
    elif section == 'merged' and not line.startswith('new_id'):
      old_ids = parts[3].split(';') if len(parts) > 3 and parts[3] else []
      merged_cones.append({
          'new_id': parts[0],
          'x': float(parts[1]),
          'y': float(parts[2]),
          'old_ids': old_ids,
      })
    elif section == 'hull' and not line.startswith('id'):
      hull_points.append(
          {'id': parts[0], 'x': float(parts[1]), 'y': float(parts[2])}
      )

  return (
      pd.DataFrame(original_cones),
      pd.DataFrame(merged_cones),
      pd.DataFrame(hull_points),
  )


orig_df, merged_df, hull_df = load_visualization_data(
    r'd:\Karusia\studia\visualization_data.csv'
)

plt.figure(figsize=(10, 10))

#pachołki na początku
if not orig_df.empty:
  plt.scatter(
      orig_df['x'],
      orig_df['y'],
      color='gray',
      s=50,
      alpha=0.5,
      label='Oryginalne pachołki',
  )
  for _, row in orig_df.iterrows():
    plt.text(
        row['x'] + 0.05,
        row['y'] + 0.05,
        row['id'],
        fontsize=8,
        color='dimgray',
    )

# pachołki scalone
if not merged_df.empty:
  plt.scatter(
      merged_df['x'],
      merged_df['y'],
      color='red',
      s=150,
      marker='^',
      edgecolors='black',
      label='Scalone pachołki (MergedCone)',
      zorder=5,
  )
  for _, row in merged_df.iterrows():
    plt.text(
        row['x'],
        row['y'] - 0.25,
        row['new_id'],
        fontsize=10,
        weight='bold',
        color='darkred',
        ha='center',
        zorder=6,
    )

# otoczka
if not hull_df.empty:
  hx = list(hull_df['x']) + [hull_df['x'].iloc[0]]
  hy = list(hull_df['y']) + [hull_df['y'].iloc[0]]

  plt.plot(
      hx,
      hy,
      color='blue',
      linewidth=2.5,
      linestyle='-',
      label='Otoczka wypukła (z C++)',
      zorder=4,
  )
  plt.fill(
      hx,
      hy,
      color='blue',
      alpha=0.1,
      label='Obszar otoczki',
      zorder=1,
  )

# wykres wygląd
plt.title(
    'Wizualizacja pachołków',
    fontsize=14,
    weight='bold',
)
plt.xlabel('Oś X')
plt.ylabel('Oś Y')
plt.grid(True, linestyle='--', alpha=0.5)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.legend(loc='upper right')
plt.tight_layout()
plt.show()