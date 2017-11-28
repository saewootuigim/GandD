# tensorflow
import numpy as np
import tensorflow as tf
import sys
import pandas as pd

def weight_variable(shape):
  """Create a weight variable with appropriate initialization."""
  initial = tf.truncated_normal(shape, stddev=0.1)
  return tf.Variable(initial)

def bias_variable(shape):
  """Create a bias variable with appropriate initialization."""
  initial = tf.constant(0.1, shape=shape)
  return tf.Variable(initial)

def variable_summaries(var):
  """Attach a lot of summaries to a Tensor (for TensorBoard visualization)."""
  mean = tf.reduce_mean(var)
  tf.summary.scalar("mean", mean)
  # with tf.name_scope("stddev"):
  stddev = tf.sqrt(tf.reduce_mean(tf.square(var - mean))) # was indented
  tf.summary.scalar("stddev", stddev)
  # tf.summary.scalar("max", tf.reduce_max(var))
  # tf.summary.scalar("min", tf.reduce_min(var))
  # tf.summary.histogram("histogram", var)

def nn_layer(input_tensor, input_dim, output_dim, layer_name, act=tf.nn.relu):
  """Reusable code for making a simple neural net layer."""
  # Adding a name scope ensures logical grouping of the layers in the graph.
  with tf.name_scope(layer_name):
    # This Variable will hold the state of the weights for the layer
    with tf.name_scope("weights"):
      weights = weight_variable([input_dim, output_dim])
      variable_summaries(weights)
    with tf.name_scope("biases"):
      biases = bias_variable([output_dim])
      variable_summaries(biases)
    # with tf.name_scope("Wx_plus_b"):
    preactivation = tf.matmul(input_tensor, weights) + biases # was indented
    # tf.summary.histogram("pre_activations", preactivation)
    activations = act(preactivation, name="activation")
    tf.summary.histogram("activations", activations)
    return activations

def feed_dict(input_x,input_y):
  """ Converts pandas dataframe of shape(n,) into a numpy ndarray of shape(1,n) """
  x_ndarray = pd.DataFrame.as_matrix(input_x)
  x_ndarray = np.reshape(x_ndarray,(1,x_ndarray.size))
  y_ndarray = np.array([input_y]).reshape(1,1)
  return {x:x_ndarray, y_:y_ndarray}


#============================================================================80
# BEGINNING OF THE MAIN
#============================================================================80
# control parameters
learning_rate = .1
n_inter = 100

# Read data set.
dataset_train = pd.read_csv("./data_dir/Pc64.00psi.csv",header=0,skipinitialspace=True)
# dataset_test  = pd.read_csv("./data_dir/Pc16.00psi_test.csv",header=0,skipinitialspace=True)

nRow, nCol = dataset_train.shape

feature = ["VoidRatio",
           "ShearStrainHP_percent_",
           "ResonantFr._Hz_",
           "ShearStrainFVD_percent_"]
X_train = dataset_train.loc[:,feature] # This is 2D matrix.
# print(X_train.shape)
y_train = dataset_train.loc[:,"Dampingfv_percent_"] # This is vector.
print(X_train.shape)
sess = tf.InteractiveSession()

# Input placeholders
with tf.name_scope("input"):
  x  = tf.placeholder(tf.float32, [None,4], name='x')
  y_ = tf.placeholder(tf.float32, [None,1], name='y')

y = nn_layer(x, 4, 1, "layer1")

with tf.name_scope("cross_entropy"):
  # The raw formulation of cross-entropy,
  # We use tf.nn.softmax_cross_entropy_with_logits on the
  # raw outputs of the nn_layer above, and then average across
  # the batch.
  print("x")
  print(x.shape)
  print("X_train.loc[i,:]")
  print(X_train.loc[0,:].shape)
  print("y_")
  print(y_.shape)
  print("y")
  print(y.shape)
  diff = tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y)
  with tf.name_scope("total"):
    cross_entropy = tf.reduce_mean(diff)
tf.summary.scalar("cross_entropy", cross_entropy)

with tf.name_scope("train"):
  train_step = tf.train.AdamOptimizer(learning_rate).minimize(cross_entropy)
with tf.name_scope("accuracy"):
  with tf.name_scope("correct_prediction"):
    correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
  with tf.name_scope("accuracy"):
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
tf.summary.scalar("accuracy", accuracy)
# Merge all the summaries and write them out to
# /tmp/tensorflow/mnist/logs/mnist_with_summaries (by default)
merged = tf.summary.merge_all()
train_writer = tf.summary.FileWriter("./log_dir/train", sess.graph)
tf.global_variables_initializer().run()

# Train.
for i in range(33):
  # if i % 10 == 0:  # Record execution stats
  #   run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
  #   run_metadata = tf.RunMetadata()
  #   summary, _ = sess.run(fetches=[merged, train_step],
  #                         feed_dict={x:X_train, y_:y_train},
  #                         options=run_options,
  #                         run_metadata=run_metadata)
  #   train_writer.add_run_metadata(run_metadata,"step%03d" % i)
  #   print("Adding run metadata for",i)
  summary, _ = sess.run(  fetches=[merged, train_step],
                          feed_dict=feed_dict(X_train.loc[i,:],y_train.loc[i]))
  train_writer.add_summary(summary, i)

train_writer.close()