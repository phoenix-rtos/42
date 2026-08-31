import pandas as pd
import numpy as np

def calculate_vector_angle(df):
    """
    Calculates the angle in degrees between two sets of vectors in a DataFrame.

    Args:
        df (pd.DataFrame): DataFrame with columns 'X', 'Y', 'Z'
                           and 'AlgoX', 'AlgoY', 'AlgoZ'.

    Returns:
        pd.DataFrame: The input DataFrame with a new 'Angle_deg' column.
    """
    # Extract vector components into NumPy arrays
    v1 = df[['X', 'Y', 'Z']].values
    v2 = df[['AlgoX', 'AlgoY', 'AlgoZ']].values

    # Calculate the dot product for each row
    dot_product = np.sum(v1 * v2, axis=1)

    # Calculate the magnitude (norm) of each vector for each row
    norm_v1 = np.linalg.norm(v1, axis=1)
    norm_v2 = np.linalg.norm(v2, axis=1)

    # Calculate the cosine of the angle
    # Use np.clip to prevent floating point errors from pushing the value
    # outside the valid arccos range of [-1, 1]
    cosine_angle = np.clip(dot_product / (norm_v1 * norm_v2), -1.0, 1.0)

    # Calculate the angle in degrees
    angle_deg = np.rad2deg(np.arccos(cosine_angle))

    # Add the result as a new column to the DataFrame
    df['Angle_deg'] = angle_deg

    # move to -5, 175 range
    df['Angle_deg'] = ((df['Angle_deg'] + 10) % 180) - 10
    
    return df


df = pd.read_csv('SunVecs_Reconstruction_Error.csv')

# 2. Calculate the angles
result_df = calculate_vector_angle(df)

# statistical properties of 'Angle_deg' column
mean_angle = result_df['Angle_deg'].mean()
median_angle = result_df['Angle_deg'].median()
std_angle = result_df['Angle_deg'].std()
# RMS - sliding window over 100 samples
rms_angle = result_df['Angle_deg'].rolling(window=100).apply(lambda x: np.sqrt(np.mean(x**2))).iloc[-1]
print(f"Mean Angle: {mean_angle}")
print(f"Median Angle: {median_angle}")
print(f"Standard Deviation of Angle: {std_angle}")
print(f"RMS Angle (last 100 samples): {rms_angle}")

# 3. Plot the results
import matplotlib.pyplot as plt

plt.figure(figsize=(10, 6))
plt.plot(result_df['Angle_deg'])
plt.xlabel('Index')
plt.ylabel('Angle (degrees)')
plt.title('Vector Angle Over Time')
plt.grid(True)
plt.show()


